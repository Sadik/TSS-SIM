#include "NOT.h"

NOT::NOT(unsigned i_, unsigned o_)
{
    i = i_;
    o = o_;
}

void NOT::prettyPrint()
{
    using namespace std;
    cout << "            i:" << i << " " << "o:" << o << endl;
}
