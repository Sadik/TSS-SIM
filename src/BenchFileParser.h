#pragma once

#include <iostream>
#include <string>
#include <fstream>

class BenchFileParser
{
public:
    BenchFileParser();
private:
    unsigned m_inputs;
    unsigned m_outputs;
    unsigned m_dff;
    unsigned m_inverter;
    unsigned m_ANDs;
    unsigned m_ORs;
    unsigned m_NANDs;
    unsigned m_NORs;
    unsigned m_buffers;
private:
    void parseFile(std::string inputFile);
    unsigned count_inputs(std::string inputFile);
    void read_gates(std::string gateLine);
    void read_header(std::string inputFile);
};
