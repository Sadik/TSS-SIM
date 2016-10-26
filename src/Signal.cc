#include "Signal.h"

#include <iostream>

Signal::Signal()
{
    std::cout << "[WRN] Signal without name was created" << std::endl;
}

Signal::Signal(std::string name, bool isPrimary)
{
    m_name = name;
    m_isPrimary = isPrimary;
}

bool Signal::isPrimary()
{
    return m_isPrimary;
}

void Signal::setIsPrimary(bool isPrimary)
{
    m_isPrimary = isPrimary;
}

std::string Signal::name() const
{
    return m_name;
}

void Signal::setName(const std::string &name)
{
    m_name = name;
}

Gate *Signal::source() const
{
    return m_source;
}

void Signal::setSource(Gate *source)
{
    m_source = source;
}

Gate* Signal::target() const
{
    return m_dest;
}

void Signal::setTarget(Gate *target)
{
    m_dest = target;
}
