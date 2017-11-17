#include "Netlist.h"

#include "SignalValue.h"
#include "Fanout.h"

Netlist::Netlist()
{

}

void Netlist::createFaults()
{
    BOOST_FOREACH(auto s, m_allSignals) {
        shared_ptr<Gate> target = s->target();
        if (boost::dynamic_pointer_cast<AND>(target) != nullptr)
        {
            m_allFaults.push_back(new SAFault(1, s));
            // sa0 for AND-input is equivalent to sa0 at AND-output
        } else if (boost::dynamic_pointer_cast<NAND>(target) != nullptr)
        {
            m_allFaults.push_back(new SAFault(1, s));
        } else if (boost::dynamic_pointer_cast<OR>(target) != nullptr)
        {
            m_allFaults.push_back(new SAFault(0, s));
        } else if (boost::dynamic_pointer_cast<NOR>(target) != nullptr)
        {
            m_allFaults.push_back(new SAFault(0, s));
        } else if (boost::dynamic_pointer_cast<NOT>(target) != nullptr)
        {
            // nothing
        } else
        {
            m_allFaults.push_back(new SAFault(1, s));
            m_allFaults.push_back(new SAFault(0, s));
        }
    }
}

std::vector< shared_ptr<Signal> > Netlist::simulate(const boost::dynamic_bitset<> &testPattern)
{
    //std::cout << "    [DEBUG] started simulating a pattern" << std::endl;
    if (m_primaryInputs.size() < 1 && m_primaryOutputs.size() < 1)
    {
        std::cout << "[WRN] tried to compute before parsing! Return." << std::endl;
        return std::vector< shared_ptr<Signal> >();
    }

    std::vector < shared_ptr<Gate> > allGates = m_allGates;
    assignPrimaryInputValues(testPattern);

    /* when a gate's output signal is set, the gate is deleted from this vector */
    while(!allGates.empty())
    {
        BOOST_FOREACH(auto currentGate, allGates)
        {
            if (currentGate && currentGate->allInputsSet())
            {
                currentGate->output()->setValue(currentGate->compute());
                /**
                 * currentGate should be done by now. It's output signal value is set
                 * and the output signal value is propagated to connected gates.
                 * Delete the current gate from allGates.
                 */
                auto it = std::find(allGates.begin(), allGates.end(), currentGate);
                if (it != allGates.end())
                {
                    allGates.erase(it);
                }
            }
        }
    }

    /* set primary output values */
    BOOST_FOREACH(auto g, allGates)
    {
        if (g->hasPrimOutput())
        {
            BOOST_FOREACH(auto s, primaryOutputs())
            {
                if (g->output()->name() == s->name())
                {
                    s->setValue(g->output()->value());
                }
            }
        }
    }

    std::vector< shared_ptr<Signal> > collect;
    BOOST_FOREACH(auto s, primaryOutputs())
    {
        shared_ptr<Signal> t = boost::make_shared<Signal>(s->name(), s->isPrimary());
        t->setValue(s->value());
        collect.push_back(t);
       // std::cout << "output: " << s->name() << " : " << s->value() << std::endl;
    }
    //std::cout << "" << std::endl;

    //std::sort (result.begin(), result.end(), sortSignals);
    resetValues();

    //std::cout << "    [DEBUG] finished simulating a pattern" << std::endl;

    return collect;
}

bool Netlist::sortSignals (shared_ptr<Signal> i, shared_ptr<Signal> j) {
    return (i->name() < j->name());
}

boost::unordered_set<shared_ptr<Signal>, SignalHash, SignalEqual> Netlist::getAllSignals() const
{
    return m_allSignals;
}

std::vector<SAFault *> Netlist::getAllFaults() const
{
    return m_allFaults;
}

void Netlist::startSimulation(const std::vector<boost::dynamic_bitset<>> &testPattern)
{
    std::cout << "[INFO] starting simulation" << std::endl;

    BOOST_FOREACH(auto pattern, testPattern)
    {
        compute(pattern);
    }

    std::cout << "[RESULT] detected faults: " << m_detectedFaults.size() << std::endl;
    float coverage = (float)m_detectedFaults.size() / (float)m_allFaults.size();
    std::cout << "[RESULT] coverage:        " << coverage << std::endl;
}

bool Netlist::differsFromGoodResult(const std::vector< shared_ptr<Signal> > result) const
{
    for (int i=0; i<result.size(); i++) {
        if (result[i]->value() != m_goodResult[i]->value()) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Netlist::compute
 * has not much to do with computation. Should rather be called simulate or something
 *
 * first simulates the netlist with testPattern as input values
 * then creates stuck-at-faults for the signals and checks, if it makes a difference
 *
 * @param testPattern inputs as specified in vec
 */
void Netlist::compute(const boost::dynamic_bitset<> &testPattern)
{
    m_goodResult = simulate(testPattern);

    std::vector< shared_ptr<Signal> > currentResult;

    BOOST_FOREACH(auto s, m_allSignals) {

        SAFault* sa0 = getFaultByName(s->name()+"-sa0");
        s->setFault(sa0); //stuck at 0
        currentResult = simulate(testPattern);
        //std::sort (currentResult.begin(), currentResult.end(), sortSignals);
        if (differsFromGoodResult(currentResult)) {
            if ( std::find(m_detectedFaults.begin(), m_detectedFaults.end(), sa0) == m_detectedFaults.end() )
               m_detectedFaults.push_back(sa0);
        }
        resetValues();

        SAFault* sa1 = getFaultByName(s->name()+"-sa1");
        s->setFault(sa1); //stuck at 1
        currentResult = simulate(testPattern);
        //std::sort (currentResult.begin(), currentResult.end(), sortSignals);
        if (differsFromGoodResult(currentResult)) {
            if ( std::find(m_detectedFaults.begin(), m_detectedFaults.end(), sa1) == m_detectedFaults.end() )
               m_detectedFaults.push_back(sa1);
        }
        resetValues();
    }

}

SAFault* Netlist::getFaultByName(std::string name) {
    BOOST_FOREACH(SAFault* fault, m_allFaults) {
        if (fault->name() == name)
            return fault;
    }
    return nullptr;
}

/**
 * @brief ::Netlist::resetValues
 * reset all signals of this netlist so the next test pattern can be simulated.
 */
void::Netlist::resetValues()
{
    BOOST_FOREACH(auto s, m_allSignals) {
        s->reset();
    }

    BOOST_FOREACH(auto s, m_ANDs)
    {
        s->reset();
    }
    BOOST_FOREACH(auto s, m_NANDs)
    {
        s->reset();
    }
    BOOST_FOREACH(auto s, m_ORs)
    {
        s->reset();
    }
    BOOST_FOREACH(auto s, m_NORs)
    {
        s->reset();
    }
    BOOST_FOREACH(auto s, m_NOTs)
    {
        s->reset();
    }
    BOOST_FOREACH(auto s, m_BUFs)
    {
        s->reset();
    }
}

/**
 * @brief assigns values of test pattern to primary inputs
 * @param testPattern one test pattern line, i.e. "10010"
 */
void Netlist::assignPrimaryInputValues(const boost::dynamic_bitset<> &testPattern)
{
    for (unsigned i = 0; i<m_primaryInputs.size(); i++)
    {
        BitValue v = ZERO;
        if (testPattern[i]==1)
            v = ONE;
        m_primaryInputs[i]->setValue(v);
        m_primaryInputs[i]->setInitSet(true); //TODO: do we need this init_set bool?
    }
}

std::vector<shared_ptr<Signal>> Netlist::primaryInputs() const
{
    return m_primaryInputs;
}

std::vector<shared_ptr<Signal>> Netlist::primaryOutputs() const
{
    return m_primaryOutputs;
}

void Netlist::addSignal(shared_ptr<Signal> newSignal)
{
    // wenn es das signal schon gibt, nicht hinzufügen
//    auto it = std::find_if( m_allSignals.begin(), m_allSignals.end(),
//                  boost::bind( & Signal::compare,
//                               _1,
//                               newSignal->name() ) );
    auto it = std::find( m_allSignals.begin(), m_allSignals.end(), newSignal);
    if (it == m_allSignals.end())
    {
        m_allSignals.insert(newSignal);
    } //TODO: CHECK IF TRUE
}

void Netlist::addPrimaryInput(shared_ptr<Signal> s)
{
    m_primaryInputs.push_back(s);
    addSignal(s);
}

void Netlist::addPrimaryOutput(shared_ptr<Signal> s)
{
    m_primaryOutputs.push_back(s);
    addSignal(s);
}

/**
 * @brief Netlist::prepare
 * must be called after bench file is fully parsed and the gates are already stored
 *
 * - sets the flag hasPrimaryOutput to gates with primary output
 * - when output signal of gate a is equal to input signal of gate b, make it one object
 * - create list of all signals m_allSignals
 * - creates a list of faults
 */
void Netlist::prepare()
{
    //creates faults and stores them in m_allFaults
    // must be called after m_allSignals is filled
    createFaults();

    /*std::cout << "[DEBUG] Signal names: " << std::endl;
    BOOST_FOREACH(Signal*s, m_allSignals) {
        std::cout << "      Signal name: " << s->name() << std::endl;
    }*/
}

shared_ptr<Signal> Netlist::primaryInputByName(std::string name)
{
    BOOST_FOREACH(auto s, m_primaryInputs)
    {
        if (s->name() == name)
            return s;
    }
    return nullptr;
}

shared_ptr<Signal> Netlist::signalByName(const std::string& name)
{

    auto it = std::find_if( m_allSignals.begin(), m_allSignals.end(),
                  boost::bind( & Signal::compare,
                               _1,
                               name ) );
    if (it != m_allSignals.end())
    {
        return *it;
    } else {
        return nullptr;
    }
}

shared_ptr<Signal> Netlist::primaryOutputByName(std::string name)
{
    BOOST_FOREACH(auto s, m_primaryOutputs)
    {
        if (s->name() == name)
            return s;
    }
    return nullptr;
}

std::vector< shared_ptr<AND> > Netlist::ANDs() const
{
    return m_ANDs;
}

std::vector< shared_ptr<BUF> > Netlist::BUFs() const
{
    return m_BUFs;
}

std::vector< shared_ptr<NAND> > Netlist::NANDs() const
{
    return m_NANDs;
}

std::vector< shared_ptr<NOR> > Netlist::NORs() const
{
    return m_NORs;
}

std::vector< shared_ptr<NOT> > Netlist::NOTs() const
{
    return m_NOTs;
}

std::vector< shared_ptr<OR> > Netlist::ORs() const
{
    return m_ORs;
}

void Netlist::addAND(shared_ptr<AND> a)
{
    m_ANDs.push_back(a);
}

void Netlist::addNAND(shared_ptr<NAND> n)
{
    m_NANDs.push_back(n);
    m_allGates.push_back(n);
}

void Netlist::addOR(shared_ptr<OR> o)
{
    m_ORs.push_back(o);
}

void Netlist::addNOR(shared_ptr<NOR> no)
{
    m_NORs.push_back(no);
}

void Netlist::addNOT(shared_ptr<NOT> nt)
{
    m_NOTs.push_back(nt);
}

void Netlist::addBUF(shared_ptr<BUF> buf)
{
    m_BUFs.push_back(buf);
}

void Netlist::addDFF(shared_ptr<DFF> dff)
{
    m_DFFs.push_back(dff);
}

void Netlist::prettyPrintInfos()
{

    cout << "[STAT] " << endl;
    cout << "       inputs:    " << m_primaryInputs.size() << endl;
    cout << "       outputs:   " << m_primaryOutputs.size() << endl;
    cout << "       inverters: " << m_NOTs.size() << endl;
    cout << "       AND Gates: " << m_ANDs.size() << endl;
    cout << "       OR Gates:  " << m_ORs.size() << endl;
    cout << "       NAND Gates:" << m_NANDs.size() << endl;
    cout << "       NOR Gates: " << m_NORs.size() << endl;
    cout << "       buffers:   " << m_BUFs.size() << endl;
    cout << "       DFFs:      " << m_DFFs.size() << endl;


    if (primaryInputs().size() >= 1){
        cout << "[STAT]    inputs: ";
        BOOST_FOREACH(auto s, primaryInputs())
        {
            cout << s->name() << " ";
        }
        cout << endl;
    }
    if (primaryOutputs().size() >= 1){
        cout << "[STAT]    outputs: ";
        BOOST_FOREACH(auto s, primaryOutputs())
        {
            cout << s->name() << " ";
        }
        cout << endl;
    }
    if (ANDs().size() >= 1)
    {
        cout << "[STAT] ANDs:" << endl;
        BOOST_FOREACH(auto n, ANDs())
        {
            n->prettyPrint();
        }
    }
    if (NANDs().size() >= 1)
    {
        cout << "[STAT] NANDs:" << endl;
        BOOST_FOREACH(auto n, NANDs())
        {
            n->prettyPrint();
        }
    }
}
