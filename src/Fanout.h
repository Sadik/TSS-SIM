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
    Fanout(shared_ptr<Signal> input, boost::unordered_set<shared_ptr<Signal> > outputs);
    void addOutput(shared_ptr<Signal> s);
    void setInput(shared_ptr<Signal> s);

    boost::unordered_set<shared_ptr<Signal> > outputs() const;
    shared_ptr<Signal> input() const;

    virtual SignalValue compute();
protected:
    shared_ptr<Signal> m_input;
    boost::unordered_set< shared_ptr<Signal> > m_outputs;
private:
    bool m_hasPrimOutput;
};
