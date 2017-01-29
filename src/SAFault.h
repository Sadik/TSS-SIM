#pragma once

#include "Signal.h"

#include <string>
#include <vector>

class Signal;

class SAFault
{
public:
    SAFault(bool sa, Signal* signal);

    bool sa() const;
    void setSa(bool sa);

    bool detected() const;
    void setDetected();

    std::string name() const;

private:
    bool m_sa; // true is stuck-at-1, false is stuck-at-0
    bool m_detected;  // true if this fault has been detected
    std::string m_name;
    Signal* m_signal;
};
