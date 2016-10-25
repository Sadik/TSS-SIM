#include "Gate.h"

Gate::Gate()
{

}

Gate::Gate(std::vector<Signal *> inputs, Signal *output)
{
    m_inputs = inputs;
    m_output = output;
}

void Gate::addInput(Signal* s)
{
    m_inputs.push_back(s);
}

void Gate::addOutput(Signal* s)
{
    m_output = s;
}

std::vector<Signal*> Gate::inputs() const
{
    return m_inputs;
}

Signal *Gate::output() const
{
    return m_output;
}
