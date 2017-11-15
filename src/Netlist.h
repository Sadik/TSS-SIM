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

#include <typeinfo>

#include <boost/bind.hpp>
#include <boost/dynamic_bitset.hpp>
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
#include <boost/range/algorithm/find.hpp>
#include <boost/unordered_set.hpp>
#include <boost/shared_ptr.hpp>

using std::cout;
using std::endl;
using boost::shared_ptr;

struct SignalEqual
{
    bool operator()(const shared_ptr<Signal>s1, const shared_ptr<Signal> s2) const
    {
        return s1->name() == s2->name();
    }
};

struct SignalHash
{

    std::size_t operator()(const shared_ptr<Signal> signal) const
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
    std::vector< shared_ptr<Signal> > simulate(const boost::dynamic_bitset<> &testPattern);
    void assignPrimaryInputValues(const boost::dynamic_bitset<> &testPattern);

    std::vector< shared_ptr<Signal> > primaryInputs() const;
    std::vector< shared_ptr<Signal> > primaryOutputs() const;
    void addSignal(shared_ptr<Signal> s);
    void addPrimaryInput(shared_ptr<Signal> s);
    void addPrimaryOutput(shared_ptr<Signal> s);
    void prepare();
    shared_ptr<Signal> signalByName(const std::string& name);
    shared_ptr<Signal> primaryInputByName(std::string name);
    shared_ptr<Signal> primaryOutputByName(std::string name);

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
    bool differsFromGoodResult(const std::vector< shared_ptr<Signal> > result) const;
    SAFault *getFaultByName(std::string name);
    void prepareGatesWithPrimOutput(std::vector <Gate*> allGates);
    void resetValues();
    bool sortSignals(shared_ptr<Signal> i, shared_ptr<Signal> j);
private:
    std::vector< shared_ptr<Signal> > m_goodResult;
    std::vector< shared_ptr<Signal> > m_primaryInputs;
    std::vector< shared_ptr<Signal> > m_primaryOutputs;
    std::vector <Gate*> m_allGates;
    boost::unordered_set< shared_ptr<Signal>, SignalHash, SignalEqual> m_allSignals;
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
