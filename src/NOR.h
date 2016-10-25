#pragma once

#include <iostream>

class NOR
{
public:
    NOR(unsigned i1_, unsigned i2_, unsigned o_);
    void prettyPrint();
private:
    unsigned i1;
    unsigned i2;
    unsigned o;
};
