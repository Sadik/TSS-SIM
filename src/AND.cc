#include "AND.h"

AND::AND(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void AND::AND::prettyPrint()
{
    std::cout << "[DEBUG]: inputs of this AND: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}
