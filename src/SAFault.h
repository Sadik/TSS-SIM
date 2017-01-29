#pragma once

#include "Signal.h"

class SAFault
{
public:
    SAFault(bool sa, Signal* signal);

    bool sa() const;
    void setSa(bool sa);

private:
    bool m_sa; // true is stuck-at-1, false is stuck-at-0
    Signal* m_signal;
};
