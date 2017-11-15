#pragma once

#include "Signal.h"
#include "SignalValue.h"

#include <string>
#include <vector>

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

class Signal;
class Gate : public boost::enable_shared_from_this<Gate>
{
public:
    Gate();
    Gate(std::vector< shared_ptr<Signal> > inputs, boost::shared_ptr<Signal> output);
    void addInput( shared_ptr<Signal> s);
    void replaceInput( shared_ptr<Signal> withThis, boost::shared_ptr<Signal> replaceThis);
    void setOutput( shared_ptr<Signal> s);
    bool allInputsSet() const;
    std::vector< shared_ptr<Signal> > inputs() const;
    shared_ptr<Signal> output() const;
    bool outputSet() const;
    bool hasPrimOutput() const;
    virtual SignalValue compute();
    void setHasPrimOutput(bool hasPrimOutput);
    void reset();

protected:
    std::vector< shared_ptr<Signal> > m_inputs;
    shared_ptr<Signal> m_output;
    using boost::enable_shared_from_this<Gate>::shared_from_this;
private:
    bool m_hasPrimOutput;
};
