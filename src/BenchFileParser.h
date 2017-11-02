#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <boost/dynamic_bitset.hpp>

#include "Netlist.h"

class BenchFileParser
{
public:
    BenchFileParser(const std::string benchFile, Netlist* netlist);

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

    bool m_readHeader;
    std::vector<boost::dynamic_bitset<>> m_testPattern;

private:
    void connectSignals();
    unsigned count_inputs(std::string inputFile);
    void parseBenchFile(const std::string& inputFile, Netlist* netlist);

    void parsePrims(const std::string &line, Netlist *netlist);
    void parseANDs(const std::string &line, Netlist *netlist);
    void parseNANDs(const std::string &line, Netlist *netlist);
    void parseBUFs(const std::string &line, Netlist *netlist);
    void parseDFFs(const std::string &line, Netlist *netlist);
    void parseNOTs(const std::string &line, Netlist *netlist);
    void parseNORs(const std::string &line, Netlist *netlist);
    void parseORs(const std::string &line, Netlist *netlist);

    void prettyPrintInfos();
    void read_body(const std::string &inputFile, Netlist *netlist);
    void read_gates(std::string line);
    void read_header(std::string inputFile);
    void readPatternFile(std::string patternFile);
};
