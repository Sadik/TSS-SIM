#include "NOR.h"

NOR::NOR(std::vector< boost::shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output)
    : Gate(inputs, output)
{
}

void NOR::NOR::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this NOR: ";
    BOOST_FOREACH(auto s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}

SignalValue NOR::compute()
{
    SignalValue o = m_inputs[0]->value();
    BOOST_FOREACH(auto s, m_inputs)
    {
        SignalValue o = o || s->value();
    }

    m_output->setValue(~o);
    return m_output->value();
}
