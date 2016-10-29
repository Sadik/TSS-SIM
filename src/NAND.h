#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class NAND : public Gate
{
public:
    NAND(std::vector<Signal*> inputs, Signal* output);
    void prettyPrint();
    bool compute();
};
