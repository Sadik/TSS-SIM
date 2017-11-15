#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class NOR : public Gate
{
public:
    NOR(std::vector< boost::shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output);
    void prettyPrint();
    virtual SignalValue compute();
};
