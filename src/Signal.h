#pragma once

#include "Gate.h"
#include "SAFault.h"
#include "SignalValue.h"

#include <string>
#include <bitset>

class Gate;
class SAFault;

class Signal
{
public:
    Signal();
    Signal(std::string name, bool isPrimary = false);
//    Signal(std::string name, Gate* source, Gate* target, bool isPrimary = false);

    shared_ptr<Signal> clone();

    bool isPrimary();
    void setIsPrimary(bool isPrimary);

    std::string name() const;
    void setName(const std::string &name);

    shared_ptr<Gate> source() const;
    void setSource(shared_ptr<Gate> source);

    shared_ptr<Gate> target() const;
    void setTarget(shared_ptr<Gate> target);

    SignalValue value() const;
    void setValue(SignalValue value);

    bool initSet() const;
    void setInitSet(bool init_set);

    void reset();

    SAFault *fault() const;
    void setFault(SAFault *fault);

    bool compare(const std::string& name);
    bool hasTarget() const;

    bool operator==(const shared_ptr<Signal> signal) const;
    bool operator==(const std::string &name) const;
    bool operator==(const Signal& signal) const;

private:
    SAFault* m_fault;
    std::string m_name;
    bool m_isPrimary;
    shared_ptr<Gate> m_source;
    shared_ptr<Gate> m_target;
    SignalValue m_value;
    bool m_init_set;

};
