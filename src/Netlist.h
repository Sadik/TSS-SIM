#pragma once

#include <string>
#include <vector>

#include "AND.h"
#include "BUF.h"
#include "DFF.h"
#include "NAND.h"
#include "NOR.h"
#include "NOT.h"
#include "OR.h"
#include "SAFault.h"
#include "Signal.h"

#include <boost/dynamic_bitset.hpp>
#include <boost/unordered_set.hpp>

struct SignalEqual
{
    bool operator()(const Signal* s1, const Signal* s2) const
    {
        return s1->name() == s2->name();
    }
};

struct SignalHash
{

    std::size_t operator()(const Signal* signal) const
    {

        std::string r = "";
        for (auto c : signal->name())
        {
            r += std::to_string((int)c % 10);
        }

        return std::stoi(r);
    }
};

class Netlist
{
public:
    Netlist();
    void compute(const boost::dynamic_bitset<> &testPattern);
    std::vector<Signal*> simulate(const boost::dynamic_bitset<> &testPattern);
    void assignPrimaryInputValues(const boost::dynamic_bitset<> &testPattern);

    std::vector<Signal*> primaryInputs() const;
    std::vector<Signal*> primaryOutputs() const;
    void addSignal(Signal* s);
    void addPrimaryInput(Signal* s);
    void addPrimaryOutput(Signal* s);
    void prepare();
    Signal* signalByName(const std::string& name);
    Signal* primaryInputByName(std::string name);
    Signal* primaryOutputByName(std::string name);

    std::vector<AND *> ANDs() const;
    std::vector<BUF *> BUFs() const;
    std::vector<NAND *> NANDs() const;
    std::vector<NOR *> NORs() const;
    std::vector<NOT *> NOTs() const;
    std::vector<OR *> ORs() const;

    void addAND(AND* a);
    void addBUF(BUF *buf);
    void addDFF(DFF *dff);
    void addNAND(NAND* n);
    void addNOR(NOR *no);
    void addNOT(NOT *nt);
    void addOR(OR *o);

    void startSimulation(const std::vector<boost::dynamic_bitset<> > &testPattern);
    void prettyPrintInfos();

private:
    void createFaults();
    bool differsFromGoodResult(std::vector<Signal *> result);
    SAFault *getFaultByName(std::string name);
    void prepareGatesWithPrimOutput(std::vector <Gate*> allGates);
    void resetValues();
    bool sortSignals(Signal *i, Signal *j);
private:
    std::vector<Signal*> m_goodResult;
    std::vector<Signal*> m_primaryInputs;
    std::vector<Signal*> m_primaryOutputs;
    std::vector <Gate*> m_allGates;
    boost::unordered_set<Signal*, SignalHash, SignalEqual> m_allSignals;
    std::vector <SAFault*> m_allFaults;
    std::vector <SAFault*> m_detectedFaults;
    std::vector<AND*> m_ANDs;
    std::vector<NAND*> m_NANDs;
    std::vector<OR*> m_ORs;
    std::vector<NOR*> m_NORs;
    std::vector<NOT*> m_NOTs;
    std::vector<BUF*> m_BUFs;
    std::vector<DFF*> m_DFFs;
};
