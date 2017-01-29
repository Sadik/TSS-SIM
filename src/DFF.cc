#include "DFF.h"

DFF::DFF(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void DFF::DFF::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this DFF: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}
