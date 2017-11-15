#include "SAFault.h"

inline const char * const BoolToString(bool b)
{
  return b ? "1" : "0";
}

SAFault::SAFault(bool sa, boost::shared_ptr<Signal> signal)
{
    m_sa = sa;
    m_signal = signal;
    m_name = signal->name() + "-sa" + BoolToString(sa);
    m_detected = false;
}


bool SAFault::sa() const
{
    return m_sa;
}

void SAFault::setSa(bool sa)
{
    m_sa = sa;
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
