#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BenchFileParser
{
public:
    BenchFileParser();
private:
    unsigned m_inputs;
    unsigned m_outputs;
    unsigned m_dff;
    unsigned m_inverters;
    unsigned m_ANDs;
    unsigned m_ORs;
    unsigned m_NANDs;
    unsigned m_NORs;
    unsigned m_buffers;

    std::vector<unsigned> m_i_values;
    std::vector<unsigned> m_o_values;
private:
    unsigned count_inputs(std::string inputFile);
    void parseFile(std::string inputFile);
    void prettyPrintInfos();
    void read_body(std::string inputFile);
    void read_gates(std::string line);
    void read_header(std::string inputFile);
};
