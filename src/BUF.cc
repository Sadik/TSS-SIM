#include "BUF.h"

BUF::BUF(std::vector<boost::shared_ptr<Signal>> inputs, boost::shared_ptr<Signal> output)
    : Gate(inputs, output)
{
}

void BUF::BUF::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this BUF: ";
    BOOST_FOREACH(auto s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}

SignalValue BUF::compute()
{
    m_output->setValue(m_inputs[0]->value());
    return m_output->value();
}
