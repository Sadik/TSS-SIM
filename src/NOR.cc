#include "NOR.h"

NOR::NOR(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void NOR::NOR::prettyPrint()
{
    std::cout << "[DEBUG]: inputs of this NOR: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}
