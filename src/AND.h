#pragma once

#include <iostream>

using namespace std;
class AND
{
public:
    AND(unsigned i2_, unsigned i1_, unsigned o_);
    void prettyPrint();
private:
    unsigned i1;
    unsigned i2;
    unsigned o;
};
