#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class NOR : public Gate
{
public:
    NOR(std::vector<Signal*> inputs, Signal* output);
    void prettyPrint();
    virtual SignalValue compute();
};
