#include "Gate.h"

#include <iostream>

#include <algorithm>

#include <boost/foreach.hpp>

Gate::Gate()
{

}

Gate::Gate(std::vector< boost::shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output)
    : m_inputs(inputs), m_output(output), m_hasPrimOutput(false)
{
}

void Gate::addInput(boost::shared_ptr<Signal> s)
{
    m_inputs.push_back(s);
}

void Gate::replaceInput( shared_ptr<Signal> replaceThis, shared_ptr<Signal> withThis)
{
    std::replace(m_inputs.begin(), m_inputs.end(), replaceThis, withThis);
}

void Gate::setOutput(boost::shared_ptr<Signal> s)
{
    m_output = s;
}

bool Gate::allInputsSet() const
{
    BOOST_FOREACH(auto input, m_inputs) {
        if (!input->initSet())
            return false;
    }
    return true;
}

std::vector< boost::shared_ptr<Signal> > Gate::inputs() const
{
    return m_inputs;
}

boost::shared_ptr<Signal> Gate::output() const
{
    return m_output;
}

bool Gate::outputSet() const
{
    return m_output->initSet();
}

bool Gate::hasPrimOutput() const
{
    return m_hasPrimOutput;
}

SignalValue Gate::compute()
{
    std::cout << "[WRN] this method in Gate::compute should not have been called" << std::endl;
}

void Gate::setHasPrimOutput(bool hasPrimOutput)
{
    m_hasPrimOutput = hasPrimOutput;
}

void Gate::reset()
{
    BOOST_FOREACH(auto s, m_inputs)
    {
        s->reset();
    }

    m_output->reset();
}
