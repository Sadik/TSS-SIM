#pragma once

#include <string>
#include <vector>

#include "AND.h"
#include "BUF.h"
#include "NAND.h"
#include "NOR.h"
#include "NOT.h"
#include "OR.h"
#include "SAFault.h"
#include "Signal.h"

#include <boost/dynamic_bitset.hpp>

class Netlist
{
public:
    Netlist();
    void compute(const boost::dynamic_bitset<> &testPattern);
    void assignPrimaryInputValues(const boost::dynamic_bitset<> &testPattern);

    std::vector<Signal*> primaryInputs() const;
    std::vector<Signal*> primaryOutputs() const;
    void addPrimaryInput(Signal* s);
    void addPrimaryOutput(Signal* s);
    void prepare();
    Signal* primaryInputByName(std::string name);
    Signal* primaryOutputByName(std::string name);

    std::vector<AND *> ANDs() const;
    std::vector<BUF *> BUFs() const;
    std::vector<NAND *> NANDs() const;
    std::vector<NOR *> NORs() const;
    std::vector<NOT *> NOTs() const;
    std::vector<OR *> ORs() const;

    void addAND(AND* a);
    void addNAND(NAND* n);
    void addOR(OR *o);
    void addNOR(NOR *no);
    void addNOT(NOT *nt);
    void addBUF(BUF *buf);
private:
    void createFaults();
    void prepareGatesWithPrimOutput(std::vector <Gate*> allGates);
    void resetValues();
private:
    std::vector<Signal*> m_primaryInputs;
    std::vector<Signal*> m_primaryOutputs;
    std::vector <Gate*> m_allGates;
    std::vector <Signal*> m_allSignals;
    std::vector <SAFault*> m_allFaults;
    std::vector<AND*> m_ANDs;
    std::vector<NAND*> m_NANDs;
    std::vector<OR*> m_ORs;
    std::vector<NOR*> m_NORs;
    std::vector<NOT*> m_NOTs;
    std::vector<BUF*> m_BUFs;
};
