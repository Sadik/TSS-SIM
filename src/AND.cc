#include "AND.h"

AND::AND(unsigned i2_, unsigned i1_, unsigned o_)
{
    i1 = i1_;
    i2 = i2_;
    o = o_;
}

void AND::AND::prettyPrint()
{
    using namespace std;
    cout << "            i1:" << i1 << " " << "i2:" << i2 << " " << "o:" << o << endl;
}
