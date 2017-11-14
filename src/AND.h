#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class AND : public Gate
{
public:
    AND(std::vector<Signal*> inputs, Signal* output);
    void prettyPrint();
    virtual SignalValue compute();
};
