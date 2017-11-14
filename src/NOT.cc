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

SignalValue NOT::compute()
{
    m_output->setValue(!m_inputs[0]->value());
    return m_output->value();
}
