#pragma once

#include "Signal.h"

#include <boost/unordered_set.hpp>
#include <string>
#include <vector>

class Signal;

class Fanout : public Gate
{
public:
    Fanout();
    Fanout(Signal* input, boost::unordered_set<Signal*> outputs);
    void addOutput(Signal* s);
    void setInput(Signal* s);

    boost::unordered_set<Signal*> outputs() const;
    Signal *input() const;

    virtual SignalValue compute();
protected:
    Signal* m_input;
    boost::unordered_set<Signal*> m_outputs;
private:
    bool m_hasPrimOutput;
};
