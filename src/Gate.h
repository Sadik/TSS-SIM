#pragma once

#include "Signal.h"

#include <string>
#include <vector>

class Signal;

class Gate
{
public:
    Gate();
    Gate(std::vector<Signal*> inputs, Signal* output);
    void addInput(Signal* s);
    void replaceInput(Signal* replaceThis, Signal* withThis);
    void setOutput(Signal* s);
    bool allInputsSet() const;
    std::vector<Signal*> inputs() const;
    Signal *output() const;
    bool outputSet() const;
    bool hasPrimOutput() const;
    virtual bool compute();
    void setHasPrimOutput(bool hasPrimOutput);
    void reset();

protected:
    std::vector<Signal*> m_inputs;
    Signal* m_output;
private:
    bool m_hasPrimOutput;
};
