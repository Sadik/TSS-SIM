#include "Gate.h"

#include <iostream>

#include <algorithm>

#include <boost/foreach.hpp>

Gate::Gate()
{

}

Gate::Gate(std::vector<Signal *> inputs, Signal *output)
    : m_inputs(inputs), m_output(output), m_hasPrimOutput(false)
{
    BOOST_FOREACH(Signal *s, m_inputs)
    {
        s->setTarget(this);
    }
    output->setSource(this);
}

void Gate::addInput(Signal* s)
{
    m_inputs.push_back(s);
}

void Gate::replaceInput(Signal *replaceThis, Signal *withThis)
{
    std::replace(m_inputs.begin(), m_inputs.end(), replaceThis, withThis);
}

void Gate::setOutput(Signal* s)
{
    m_output = s;
}

bool Gate::allInputsSet() const
{
    BOOST_FOREACH(Signal* input, m_inputs) {
        if (!input->initSet())
            return false;
    }
    return true;
}

std::vector<Signal*> Gate::inputs() const
{
    return m_inputs;
}

Signal *Gate::output() const
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
    BOOST_FOREACH(Signal* s, m_inputs)
    {
        s->reset();
    }

    m_output->reset();
}
