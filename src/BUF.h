#pragma once

#include <iostream>

using namespace std;
class BUF
{
public:
    BUF(unsigned i_, unsigned o_);
    void prettyPrint();
private:
    unsigned i;
    unsigned o;
};
