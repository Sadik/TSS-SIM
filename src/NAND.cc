#include "NAND.h"

NAND::NAND(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void NAND::NAND::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this NAND: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }

    std::cout << "         output: " << output()->name() << std::endl;
}

bool NAND::compute()
{
    bool o = m_inputs[0]->value();
    BOOST_FOREACH(Signal* s, m_inputs)
    {
//        std::cout << "    " << o << " & " << s->value() << std::endl;
        bool o = o && s->value();
    }

//    std::cout << "o is " << std::boolalpha << !o << std::endl;

    m_output->setValue(!o);
    return m_output->value();
}

