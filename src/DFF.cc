#include "DFF.h"

DFF::DFF(std::vector< boost::shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output)
    : Gate(inputs, output)
{
}

void DFF::DFF::prettyPrint()
{
    std::cout << "[DEBUG] inputs of this DFF: ";
    BOOST_FOREACH(auto s, inputs())
    {
        std::cout << s->name() << " ";
    }
    std::cout << "         output: " << output()->name() << std::endl;
}
