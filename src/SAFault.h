#pragma once

#include "Signal.h"

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

class Signal;

class SAFault
{
public:
    SAFault(bool sa, shared_ptr<Signal> signal);

    bool sa() const;

    bool detected() const;
    void setDetected();

    std::string name() const;

    shared_ptr<Signal> signal() const;

private:
    const bool m_sa; // true is stuck-at-1, false is stuck-at-0
    bool m_detected;  // true if this fault has been detected
    const std::string m_name;
    const shared_ptr<Signal> m_signal;
};
