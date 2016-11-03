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
    m_value = 0;
    m_init_set = 0;
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

Gate* Signal::destiny() const
{
    return m_dest;
}

void Signal::setDestiny(Gate *dest)
{
    m_dest = dest;
}

bool Signal::value() const
{
    return m_value;
}

void Signal::setValue(bool value)
{
    m_value = value;
    m_init_set = true;
}

bool Signal::initSet() const
{
    return m_init_set;
}

void Signal::setInitSet(bool init_set)
{
    m_init_set = init_set;
}
