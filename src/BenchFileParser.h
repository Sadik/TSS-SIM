#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "AND.h"
#include "BUF.h"
#include "NAND.h"
#include "NOR.h"
#include "NOT.h"
#include "OR.h"
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
    std::vector<AND*> m_ANDs;
    std::vector<NAND*> m_NANDs;
    std::vector<OR*> m_ORs;
    std::vector<NOR*> m_NORs;
    std::vector<NOT*> m_NOTs;
    std::vector<BUF*> m_BUFs;
private:
    Netlist* m_netlist;
    unsigned count_inputs(std::string inputFile);
    void parseFile(std::string inputFile);
    void prettyPrintInfos();
    void read_body(std::string inputFile);
    void read_gates(std::string line);
    void read_header(std::string inputFile);
};
