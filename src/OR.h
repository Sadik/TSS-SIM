#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class OR : public Gate
{
public:
    OR(std::vector< boost::shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output);
    void prettyPrint();
    virtual SignalValue compute();
};
