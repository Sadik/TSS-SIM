#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class DFF : public Gate
{
public:
    DFF(std::vector<Signal*> inputs, Signal* output);
    void prettyPrint();
};
