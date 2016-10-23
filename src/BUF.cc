#include "BUF.h"

BUF::BUF(unsigned i_, unsigned o_)
{
    i = i_;
    o = o_;
}

void BUF::prettyPrint()
{
    using namespace std;
    cout << "            i:" << i << " " << "o:" << o << endl;
}
