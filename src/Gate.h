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
    void addOutput(Signal* s);
    std::vector<Signal*> inputs() const;
    Signal *output() const;

private:
    std::vector<Signal*> m_inputs;
    Signal* m_output;
};
