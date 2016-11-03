#pragma once

#include "Gate.h"

#include <string>
#include <bitset>

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

    Gate *destiny() const;
    void setDestiny(Gate *dest);

    bool value() const;
    void setValue(bool value);

    bool initSet() const;
    void setInitSet(bool init_set);

private:
    std::string m_name;
    bool m_isPrimary;
    Gate* m_source;
    Gate* m_dest;
    bool m_value;
    bool m_init_set;

};
