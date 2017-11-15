#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class BUF : public Gate
{
public:
    BUF(std::vector<boost::shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output);
    void prettyPrint();
    virtual SignalValue compute();
};
