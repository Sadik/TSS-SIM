#include "NOT.h"

NOT::NOT(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void NOT::NOT::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this NOT: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}
