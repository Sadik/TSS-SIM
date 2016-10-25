#pragma once

#include <iostream>

class BUF
{
public:
    BUF(unsigned i_, unsigned o_);
    void prettyPrint();
private:
    unsigned i;
    unsigned o;
};
