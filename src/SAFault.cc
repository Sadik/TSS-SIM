#include "SAFault.h"

inline const char * const BoolToString(bool b)
{
  return b ? "1" : "0";
}

SAFault::SAFault(bool sa, boost::shared_ptr<Signal> signal)
    : m_sa(sa), m_signal(signal), m_detected(false), m_name(signal->name() + "-sa" + BoolToString(sa))
{
}


bool SAFault::sa() const
{
    return m_sa;
}

bool SAFault::detected() const
{
    return m_detected;
}


void SAFault::setDetected()
{
    m_detected = true;
}

std::string SAFault::name() const
{
    return m_name;
}

shared_ptr<Signal> SAFault::signal() const
{
    return m_signal;
}
