#include "Signal.h"

#include <iostream>

Signal::Signal()
    : m_name(""), m_isPrimary(false), m_value(ZERO), m_init_set(0), m_fault(NULL)
{
//    std::cout << "[WRN] Signal without name was created" << std::endl;
}

Signal::Signal(std::string name, bool isPrimary)
    : m_name(name), m_isPrimary(isPrimary), m_value(ZERO), m_init_set(0), m_fault(NULL)
{
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
    return m_target;
}

void Signal::setTarget(Gate *dest)
{
    m_target = dest;
}

/**
 * @brief Signal::value
 * the correct value is only returned when there is no stuck-at fault on this signal
 *
 * @return
 */
SignalValue Signal::value() const
{
    if (m_fault == NULL)
        return m_value;
    else if (m_fault->sa()) {
        return ONE;
    } else if(!m_fault->sa()) {
        return ZERO;
    }
}

void Signal::setValue(SignalValue value)
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

void Signal::reset()
{
    m_value = ZERO;
    m_init_set = false;
    m_fault = nullptr;
}

SAFault *Signal::fault() const
{
    return m_fault;
}

void Signal::setFault(SAFault *fault)
{
    m_fault = fault;
}

bool Signal::compare(const std::string &name)
{
    return this->name() == name;
}

bool Signal::operator==(const std::string& name) const
{
    return this->name() == name;
}

bool Signal::operator==(const Signal* signal) const
{
    return this->name() == signal->name();
}

bool Signal::operator==(const Signal &signal) const
{
    return this->name() == signal.name();
}
