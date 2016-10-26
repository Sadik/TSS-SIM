#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Netlist.h"

class BenchFileParser
{
public:
    BenchFileParser();
private:
    unsigned m_inputs_count;
    unsigned m_outputs_count;
    unsigned m_dff_count;
    unsigned m_inverters_count;
    unsigned m_ANDs_count;
    unsigned m_ORs_count;
    unsigned m_NANDs_count;
    unsigned m_NORs_count;
    unsigned m_buffers_count;

    std::vector<unsigned> m_inputs;
    std::vector<unsigned> m_outputs;
    Netlist* m_netlist;
    bool m_readHeader;

private:
    void connectSignals();
    unsigned count_inputs(std::string inputFile);
    void parseFile(std::string inputFile);
    void parsePrims(std::string line);
    void parseANDs(std::string line);
    void parseNANDs(std::string line);
    void parseORs(std::string line);
    void parseNORs(std::string line);
    void parseNOTs(std::string line);
    void parseBUFs(std::string line);
    void prettyPrintInfos();
    void read_body(std::string inputFile);
    void read_gates(std::string line);
    void read_header(std::string inputFile);
};
