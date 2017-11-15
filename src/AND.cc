#include "AND.h"

AND::AND(std::vector<boost::shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output)
    : Gate(inputs, output)
{
}

void AND::AND::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this AND: ";
    BOOST_FOREACH(auto s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}

SignalValue AND::compute()
{
    SignalValue o = m_inputs[0]->value();
    BOOST_FOREACH(auto s, m_inputs)
    {
        SignalValue o = o && s->value();
    }

    m_output->setValue(o);
    return m_output->value();
}
