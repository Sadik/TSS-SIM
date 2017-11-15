#pragma once

#include "Signal.h"
#include "SignalValue.h"

#include <string>
#include <vector>

#include <boost/shared_ptr.hpp>

class Signal;

class Gate
{
public:
    Gate();
    Gate(std::vector<boost::shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output);
    void addInput(boost::shared_ptr<Signal> s);
    void replaceInput(boost::shared_ptr<Signal> withThis, boost::shared_ptr<Signal> replaceThis);
    void setOutput(boost::shared_ptr<Signal> s);
    bool allInputsSet() const;
    std::vector<boost::shared_ptr<Signal> > inputs() const;
    boost::shared_ptr<Signal> output() const;
    bool outputSet() const;
    bool hasPrimOutput() const;
    virtual SignalValue compute();
    void setHasPrimOutput(bool hasPrimOutput);
    void reset();

protected:
    std::vector< boost::shared_ptr<Signal> > m_inputs;
    boost::shared_ptr<Signal> m_output;
private:
    bool m_hasPrimOutput;
};
