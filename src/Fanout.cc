#include "Fanout.h"

#include <iostream>

#include <algorithm>

#include <boost/foreach.hpp>

Fanout::Fanout()
{

}

Fanout::Fanout(shared_ptr<Signal> input, boost::unordered_set< shared_ptr<Signal> > outputs)
{
    m_input = input;
    m_outputs = outputs;
    m_hasPrimOutput = false;
}

void Fanout::addOutput(shared_ptr<Signal> s)
{
    m_outputs.insert(s);
}

void Fanout::setInput(shared_ptr<Signal> s)
{
    m_input = s;
}

shared_ptr<Signal> Fanout::input() const
{
    return m_input;
}

boost::unordered_set< shared_ptr<Signal> > Fanout::outputs() const
{
    return m_outputs;
}


SignalValue Fanout::compute()
{
    BOOST_FOREACH(shared_ptr<Signal> s, m_outputs)
    {
        s->setValue(m_input->value());
    }
}
