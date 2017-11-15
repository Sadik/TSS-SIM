#pragma once

#include "Gate.h"
#include <iostream>
#include <boost/foreach.hpp>

class DFF : public Gate
{
public:
    DFF(std::vector< boost::shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output);
    void prettyPrint();
};
