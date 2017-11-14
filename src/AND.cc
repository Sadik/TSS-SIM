#include "AND.h"

AND::AND(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void AND::AND::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this AND: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}

SignalValue AND::compute()
{
    SignalValue o = m_inputs[0]->value();
    BOOST_FOREACH(Signal* s, m_inputs)
    {
        SignalValue o = o && s->value();
    }

    m_output->setValue(o);
    return m_output->value();
}
