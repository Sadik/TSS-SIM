#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class OR : public Gate
{
public:
    OR(std::vector<Signal*> inputs, Signal* output);
    void prettyPrint();
};
