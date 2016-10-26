#pragma once

#include "Gate.h"

#include <string>

class Gate;

class Signal
{
public:
    Signal();
    Signal(std::string name, bool isPrimary = false);

    bool isPrimary();
    void setIsPrimary(bool isPrimary);

    std::string name() const;
    void setName(const std::string &name);

    Gate *source() const;
    void setSource(Gate *source);

    Gate *target() const;
    void setTarget(Gate *target);

private:
    std::string m_name;
    bool m_isPrimary;
    Gate* m_source;
    Gate* m_dest;

};
