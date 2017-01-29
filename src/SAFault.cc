#include "SAFault.h"

SAFault::SAFault(bool sa, Signal *signal)
{
    m_sa = sa;
    m_signal = signal;
}

bool SAFault::sa() const
{
    return m_sa;
}

void SAFault::setSa(bool sa)
{
    m_sa = sa;
}
