#include "Fanout.h"

#include <iostream>

#include <algorithm>

#include <boost/foreach.hpp>

Fanout::Fanout()
{

}

Fanout::Fanout(Signal *input, boost::unordered_set<Signal *> outputs)
{
    m_input = input;
    m_outputs = outputs;
    m_hasPrimOutput = false;
}

void Fanout::addOutput(Signal* s)
{
    m_outputs.insert(s);
}

void Fanout::setInput(Signal* s)
{
    m_input = s;
}

Signal * Fanout::input() const
{
    return m_input;
}

boost::unordered_set<Signal *> Fanout::outputs() const
{
    return m_outputs;
}


SignalValue Fanout::compute()
{
    BOOST_FOREACH(Signal* s, m_outputs)
    {
        s->setValue(m_input->value());
    }
}
