#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class NOT : public Gate
{
public:
    NOT(std::vector<Signal*> inputs, Signal* output);
    void prettyPrint();
    virtual bool compute();
};
