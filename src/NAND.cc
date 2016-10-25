#include "NAND.h"

NAND::NAND(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void NAND::NAND::prettyPrint()
{
    std::cout << "[DEBUG]: inputs of this NAND: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }

    std::cout << "         output: " << output()->name() << std::endl;
}
