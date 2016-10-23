#include "NOR.h"

NOR::NOR(unsigned i1_, unsigned i2_, unsigned o_)
{
    i1 = i1_;
    i2 = i2_;
    o = o_;
}

void NOR::NOR::prettyPrint()
{
    using namespace std;
    cout << "            i1:" << i1 << " " << "i2:" << i2 << " " << "o:" << o << endl;
}
