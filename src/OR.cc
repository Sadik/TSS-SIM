#include "OR.h"

OR::OR(std::vector<Signal *> inputs, Signal *output)
    : Gate(inputs, output)
{
}

void OR::OR::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this OR: ";
    BOOST_FOREACH(Signal* s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}

bool OR::compute()
{
    bool o = m_inputs[0]->value();
    BOOST_FOREACH(Signal* s, m_inputs)
    {
        bool o = o || s->value();
    }

    m_output->setValue(o);
    return m_output->value();
}
