#include "OR.h"

OR::OR(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void OR::OR::prettyPrint()
{
    std::cout << "[DEBUG]: inputs of this OR: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}
