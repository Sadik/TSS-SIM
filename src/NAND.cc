#include "NAND.h"

using namespace boost;

NAND::NAND(std::vector<shared_ptr<Signal> > inputs, shared_ptr<Signal> output)
    : Gate(inputs, output)
{
}

void NAND::NAND::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this NAND: ";
    BOOST_FOREACH(auto s, inputs())
    {
        std::cout << s->name() << " ";
    }

    std::cout << "         output: " << output()->name() << std::endl;
}

SignalValue NAND::compute()
{
    SignalValue o = m_inputs[0]->value();
    BOOST_FOREACH(auto s, m_inputs)
    {
//        std::cout << "    " << o << " & " << s->value() << std::endl;
        SignalValue o = o && s->value();
    }

//    std::cout << "o is " << std::boolalpha << !o << std::endl;

    m_output->setValue(~o);
    return m_output->value();
}

