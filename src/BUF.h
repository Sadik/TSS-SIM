#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class BUF : public Gate
{
public:
    BUF(std::vector<Signal*> inputs, Signal* output);
    void prettyPrint();
    virtual bool compute();
};
