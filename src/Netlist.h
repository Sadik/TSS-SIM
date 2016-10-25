#pragma once

#include <string>
#include <vector>

#include "Signal.h"

class Netlist
{
public:
    Netlist();
    std::vector<Signal*> primaryInputs() const;
    std::vector<Signal*> primaryOutputs() const;
    void addPrimaryInput(Signal* s);
    void addPrimaryOutput(Signal* s);
    Signal* getPrimaryInputByName(std::string name);
    Signal* getPrimaryOutputByName(std::string name);
private:
    std::vector<Signal*> m_primaryInputs;
    std::vector<Signal*> m_primaryOutputs;
};
