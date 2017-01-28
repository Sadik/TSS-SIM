#include "Netlist.h"

#include <boost/foreach.hpp>
#include <typeinfo>

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


/**
 * @brief Netlist::compute
 * has nothing to do with computation. Should be called simulate or at least propagate
 *
 * assigns values of testPattern to primary inputs
 * every output signal of every gate gets a value here. Starting with the gates directly after primary inputs.
 *
 * @param testPattern inputs as specified in vec
 */
void Netlist::compute(const boost::dynamic_bitset<> &testPattern)
{
    if (m_primaryInputs.size() < 1 && m_primaryOutputs.size() < 1)
    {
        std::cout << "[WRN] tried to compute before parsing! Return." << std::endl;
        return;
    }

    assignPrimaryInputValues(testPattern);

    std::vector <Gate*> allGates;
    std::vector <Signal*> internalSignals;
    allGates.insert( allGates.end(), m_ANDs.begin(), m_ANDs.end() );
    allGates.insert( allGates.end(), m_ORs.begin(), m_ORs.end() );
    allGates.insert( allGates.end(), m_NORs.begin(), m_NORs.end() );
    allGates.insert( allGates.end(), m_NOTs.begin(), m_NOTs.end() );
    allGates.insert( allGates.end(), m_BUFs.begin(), m_BUFs.end() );
    allGates.insert( allGates.end(), m_NANDs.begin(), m_NANDs.end() );

    //set primary outputs
    prepareGatesWithPrimOutput(allGates);

    /* when a gate's output signal is set, the gate is deleted from this vector */
    while(!allGates.empty())
    {

        /** We want to start with gates directly after primary inputs, but the order in allGates is unknown
         * though the first gates are likely at the end.*/
        BOOST_REVERSE_FOREACH(Gate* currentGate, allGates)
        {
//            std::cout << "remaining gates: " << std::endl;
//            BOOST_FOREACH(Gate* r, allGates)
//            {
//                std::cout << "  " << r->output()->name();
//            }

//            std::cout << std::endl;
//            std::cout << "################################" << std::endl;
//            std::cout << "remaining gates: " << allGates.size() << std::endl;
//            std::cout << "current gate: " << currentGate->output()->name() << std::endl;
//            std::cout << "gates inputs are set: " << currentGate->allInputsSet() << std::endl;
            if (currentGate->allInputsSet())
            {

                currentGate->output()->setValue(currentGate->compute()); //TODO: necessary?
//                std::cout << "    handling gate with output " << currentGate->output()->name() << "("<< currentGate->output()->value() << ")" << std::endl;

                /**
                 * alle inputs aller gates absuchen und input value speichern.
                 * propagate the output value of the current gate to the connected gates.
                 */
                BOOST_FOREACH(Gate* g, allGates)
                {
                    if (g->output()->name() == currentGate->output()->name())
                        continue;
                    BOOST_FOREACH(Signal* s, g->inputs())
                    {
                        if (s->isPrimary())
                            continue;
                        if (s->name() == currentGate->output()->name())
                        {
//                            std::cout <<  "    output " << currentGate->output()->name() << " is input of gate " <<  g->output()->name() << std::endl;
                            s->setValue(currentGate->output()->value());
                        }
                    }
                }

//                BOOST_FOREACH(Signal* gateInput, currentGate->inputs())
//                {
//                    std::cout << "    input: " << gateInput->name() << " " << gateInput->value() << " " << gateInput->initSet() << std::endl;
//                }

                /**
                 * currentGate should be done by now. It's output signal value is set and the output signal value is propagated to connected gates.
                 * delete the current gate from allGates.
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

    BOOST_FOREACH(Signal*s, primaryOutputs())
    {
        std::cout << "output: " << s->name() << " : " << s->value() << std::endl;
    }
    std::cout << "" << std::endl;

    resetValues();
}

/**
 * @brief ::Netlist::resetValues
 * reset all signals of this netlist so the next test pattern can be simulated.
 */
void::Netlist::resetValues()
{
    BOOST_FOREACH(Signal* s, m_primaryInputs)
    {
        s->reset();
    }
    BOOST_FOREACH(Signal* s, m_primaryOutputs)
    {
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
        m_primaryInputs[i]->setValue((bool)testPattern[i]);
        m_primaryInputs[i]->setInitSet(true); //TODO: do we need this init_set bool?
    }

    std::cout << "INPUT: ";
    BOOST_FOREACH(Signal* inp, m_primaryInputs)
    {
        std::cout << inp->value();
    }
    std::cout << std::endl;
//    std::cout << "INPUT0: " << m_primaryInputs[0]->name() <<  " = " << m_primaryInputs[0]->value() << std::endl;
//    std::cout << "INPUT1: " << m_primaryInputs[1]->name() <<  " = " << m_primaryInputs[1]->value() << std::endl;
//    std::cout << "INPUT2: " << m_primaryInputs[2]->name() <<  " = " << m_primaryInputs[2]->value() << std::endl;
//    std::cout << "INPUT3: " << m_primaryInputs[3]->name() <<  " = " << m_primaryInputs[3]->value() << std::endl;
//    std::cout << "INPUT4: " << m_primaryInputs[4]->name() <<  " = " << m_primaryInputs[4]->value() << std::endl;
}

std::vector<Signal*> Netlist::primaryInputs() const
{
    return m_primaryInputs;
}

std::vector<Signal*> Netlist::primaryOutputs() const
{
    return m_primaryOutputs;
}

void Netlist::addPrimaryInput(Signal* s)
{
    m_primaryInputs.push_back(s);
}

void Netlist::addPrimaryOutput(Signal* s)
{
    m_primaryOutputs.push_back(s);
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
