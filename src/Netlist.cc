#include "Netlist.h"

#include "SignalValue.h"
#include "Fanout.h"

#include <boost/foreach.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/bind.hpp>
#include <typeinfo>

using namespace std;

Netlist::Netlist()
{

}

/**
 * @brief Netlist::prepareGatesWithPrimOutput
 * sets the bool isPrimOutput to all outputs of gate.
 * TODO: hasn't this been done already?
 *
 * @param allGates
 */
void Netlist::prepareGatesWithPrimOutput(std::vector <Gate*> allGates)
{
    BOOST_FOREACH(Gate* g, allGates)
    {
        BOOST_FOREACH(Signal*s, primaryOutputs())
        {
            if (s->name() == g->output()->name())
            {
                g->setHasPrimOutput(true);
            }
        }
    }
}

void Netlist::createFaults()
{
    BOOST_FOREACH(Signal*s, m_allSignals) {
        m_allFaults.push_back(new SAFault(1, s));
        m_allFaults.push_back(new SAFault(0, s));
    }
}

std::vector<Signal *> Netlist::simulate(const boost::dynamic_bitset<> &testPattern)
{
    //std::cout << "    [DEBUG] started simulating a pattern" << std::endl;
    if (m_primaryInputs.size() < 1 && m_primaryOutputs.size() < 1)
    {
        std::cout << "[WRN] tried to compute before parsing! Return." << std::endl;
        return std::vector<Signal*>();
    }

    std::vector <Gate*> allGates = m_allGates;
    assignPrimaryInputValues(testPattern);

    /* when a gate's output signal is set, the gate is deleted from this vector */
    while(!allGates.empty())
    {
        BOOST_FOREACH(Gate* currentGate, allGates)
        {
            if (currentGate->allInputsSet())
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
    BOOST_FOREACH(Gate* g, allGates)
    {
        if (g->hasPrimOutput())
        {
            BOOST_FOREACH(Signal* s, primaryOutputs())
            {
                if (g->output()->name() == s->name())
                {
                    s->setValue(g->output()->value());
                }
            }
        }
    }

    std::vector<Signal*> collect;
    BOOST_FOREACH(Signal*s, primaryOutputs())
    {
        Signal* t = new Signal(s->name(), s->isPrimary());
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

bool Netlist::sortSignals (Signal* i, Signal* j) {
    return (i->name() < j->name());
}

void Netlist::startSimulation(const std::vector<boost::dynamic_bitset<>> &testPattern)
{
    std::cout << "[INFO] starting simulation" << std::endl;

    BOOST_FOREACH(auto pattern, testPattern)
    {
        compute(pattern);
    }

    std::cout << "[RESULT] total faults:    " << m_allFaults.size() << std::endl;
    std::cout << "[RESULT] detected faults: " << m_detectedFaults.size() << std::endl;
    float coverage = (float)m_detectedFaults.size() / (float)m_allFaults.size();
    std::cout << "[RESULT] coverage:        " << coverage << std::endl;
}

bool Netlist::differsFromGoodResult(const std::vector<Signal*> result) const
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

    std::vector<Signal*> currentResult;

    BOOST_FOREACH(Signal*s, m_allSignals) {

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
    BOOST_FOREACH(Signal*s, m_allSignals) {
        s->reset();
    }

    BOOST_FOREACH(Gate* s, m_ANDs)
    {
        s->reset();
    }
    BOOST_FOREACH(Gate* s, m_NANDs)
    {
        s->reset();
    }
    BOOST_FOREACH(Gate* s, m_ORs)
    {
        s->reset();
    }
    BOOST_FOREACH(Gate* s, m_NORs)
    {
        s->reset();
    }
    BOOST_FOREACH(Gate* s, m_NOTs)
    {
        s->reset();
    }
    BOOST_FOREACH(Gate* s, m_BUFs)
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

std::vector<Signal*> Netlist::primaryInputs() const
{
    return m_primaryInputs;
}

std::vector<Signal*> Netlist::primaryOutputs() const
{
    return m_primaryOutputs;
}

void Netlist::addSignal(Signal *s)
{
    // wenn es das signal schon gibt
    // erstelle Fanout
    //
//    auto it = std::find( m_allSignals.begin(), m_allSignals.end(), s);
    auto it = std::find_if( m_allSignals.begin(), m_allSignals.end(),
                  boost::bind( & Signal::compare,
                               _1,
                               s->name() ) );
    if (it != m_allSignals.end())
    {
        Signal* foundSignal = *it;
        Gate* oldTarget = foundSignal->target();
        if (!oldTarget)
        {
            // this signal has not been connected to the left, probably primary
            // thus no fanout needed here
            m_allSignals.insert(s);
            return;
        }

        Fanout* f = new Fanout();
        f->setInput(foundSignal);
        foundSignal->setTarget(f);

        Signal* afterFanout = new Signal(foundSignal->name() + "_" + std::to_string(m_allSignals.size()));
        afterFanout->setTarget(oldTarget);
        afterFanout->setSource(f);

        boost::unordered_set<Signal*> fanoutOutputs;
        fanoutOutputs.insert(afterFanout);
        fanoutOutputs.insert(s);

        m_allSignals.insert(afterFanout);

    } else
    {
        m_allSignals.insert(s);
    }
}

void Netlist::addPrimaryInput(Signal* s)
{
    m_primaryInputs.push_back(s);
    addSignal(s);
}

void Netlist::addPrimaryOutput(Signal* s)
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
    //set hasPrimaryOutput
    prepareGatesWithPrimOutput(m_allGates);

    m_allGates.insert( m_allGates.end(), m_ANDs.begin(), m_ANDs.end() );
    m_allGates.insert( m_allGates.end(), m_ORs.begin(), m_ORs.end() );
    m_allGates.insert( m_allGates.end(), m_NORs.begin(), m_NORs.end() );
    m_allGates.insert( m_allGates.end(), m_NOTs.begin(), m_NOTs.end() );
    m_allGates.insert( m_allGates.end(), m_BUFs.begin(), m_BUFs.end() );
    m_allGates.insert( m_allGates.end(), m_NANDs.begin(), m_NANDs.end() );
    //the DFFs have been collected, but we don't treat them as a gate (actually DFF is a flip flop, but in my case I don't need a new class only for the correct terminology)

    BOOST_FOREACH(Signal*s, m_primaryInputs) {
        m_allSignals.insert(s);
    }

    BOOST_FOREACH(Signal*s, m_primaryOutputs) {
        m_allSignals.insert(s);
    }

    // Für jedes input signal des Gates schauen wir, ob es ein output signal eines anderen gates ist.
    // Dann verbinden wir sie.
    BOOST_FOREACH(Gate*g, m_allGates) {
        BOOST_FOREACH(Signal *s, g->inputs()) {
            BOOST_FOREACH(Gate*h, m_allGates) {
                if (g == h)
                    continue;
                if (s->name() == h->output()->name()) {
                    g->replaceInput(s, h->output());
                }
            }
        }
    }

    //add signals to m_allSignals if they aren't already in the list (vector)
    BOOST_FOREACH(Gate*g, m_allGates) {
        if ( std::find(m_allSignals.begin(), m_allSignals.end(), g->output()) == m_allSignals.end() )
           m_allSignals.insert(g->output());

        BOOST_FOREACH(Signal*s, g->inputs()) {
            if ( std::find(m_allSignals.begin(), m_allSignals.end(), s) == m_allSignals.end() )
               m_allSignals.insert(s);
        }

    }

    //creates faults and stores them in m_allFaults
    // must be called after m_allSignals is filled
    createFaults();

    /*std::cout << "[DEBUG] Signal names: " << std::endl;
    BOOST_FOREACH(Signal*s, m_allSignals) {
        std::cout << "      Signal name: " << s->name() << std::endl;
    }*/
}

Signal *Netlist::primaryInputByName(std::string name)
{
    BOOST_FOREACH(Signal* s, m_primaryInputs)
    {
        if (s->name() == name)
            return s;
    }
    return nullptr;
}

Signal *Netlist::signalByName(const std::string& name)
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

Signal *Netlist::primaryOutputByName(std::string name)
{
    BOOST_FOREACH(Signal* s, m_primaryOutputs)
    {
        if (s->name() == name)
            return s;
    }
    return nullptr;
}

std::vector<AND *> Netlist::ANDs() const
{
    return m_ANDs;
}

std::vector<BUF *> Netlist::BUFs() const
{
    return m_BUFs;
}

std::vector<NAND *> Netlist::NANDs() const
{
    return m_NANDs;
}

std::vector<NOR *> Netlist::NORs() const
{
    return m_NORs;
}

std::vector<NOT *> Netlist::NOTs() const
{
    return m_NOTs;
}

std::vector<OR *> Netlist::ORs() const
{
    return m_ORs;
}

void Netlist::addAND(AND *a)
{
    m_ANDs.push_back(a);
}

void Netlist::addNAND(NAND *n)
{
    m_NANDs.push_back(n);
}

void Netlist::addOR(OR *o)
{
    m_ORs.push_back(o);
}


void Netlist::addNOR(NOR *no)
{
    m_NORs.push_back(no);
}

void Netlist::addNOT(NOT *nt)
{
    m_NOTs.push_back(nt);
}

void Netlist::addBUF(BUF *buf)
{
    m_BUFs.push_back(buf);
}

void Netlist::addDFF(DFF *dff)
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
        BOOST_FOREACH(Signal* s, primaryInputs())
        {
            cout << s->name() << " ";
        }
        cout << endl;
    }
    if (primaryOutputs().size() >= 1){
        cout << "[STAT]    outputs: ";
        BOOST_FOREACH(Signal* s, primaryOutputs())
        {
            cout << s->name() << " ";
        }
        cout << endl;
    }
    if (ANDs().size() >= 1)
    {
        cout << "[STAT] ANDs:" << endl;
        BOOST_FOREACH(AND* n, ANDs())
        {
            n->prettyPrint();
        }
    }
    if (NANDs().size() >= 1)
    {
        cout << "[STAT] NANDs:" << endl;
        BOOST_FOREACH(NAND* n, NANDs())
        {
            n->prettyPrint();
        }
    }
}
