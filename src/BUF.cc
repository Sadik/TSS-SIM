#include "BUF.h"

BUF::BUF(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void BUF::BUF::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this BUF: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}
