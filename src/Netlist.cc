#include "Netlist.h"

#include <boost/foreach.hpp>

Netlist::Netlist()
{

}

void Netlist::compute(std::vector<boost::dynamic_bitset<>> testPattern)
{
    if (m_primaryInputs.size() < 1 && m_primaryOutputs.size() < 1)
    {
        std::cout << "[WRN] tried to compute before parsing! Return." << std::endl;
        return;
    }

//    std::cout<<"bit is of type: "<<typeid(testPattern[0][0]).name()<<std::endl;
    boost::dynamic_bitset<> test1 = testPattern[0];
    for (unsigned i = 0; i<m_primaryInputs.size(); i++)
    {
        m_primaryInputs[i]->setValue((bool)test1[i]);
    }

    std::cout << "INPUT0: " << m_primaryInputs[0]->value() << std::endl;
    std::cout << "INPUT1: " << m_primaryInputs[1]->value() << std::endl;
    std::cout << "INPUT2: " << m_primaryInputs[2]->value() << std::endl;
    std::cout << "INPUT3: " << m_primaryInputs[3]->value() << std::endl;
    std::cout << "INPUT4: " << m_primaryInputs[4]->value() << std::endl;


//    BOOST_FOREACH(boost::dynamic_bitset<> bits, testPattern)
//    {

//    }

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
