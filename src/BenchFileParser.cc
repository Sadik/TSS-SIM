/**
 * @file   BenchFileParser.cc
 * @Author Sadik Özoguz (sadik@informatik.uni-bremen.de)
 * @date   Oktober, 2016
 * @brief  Parses a bench file hopefully
 *
 * Some time it will...
 */

#include "BenchFileParser.h"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/home/classic/actor.hpp>
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_match.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/version.hpp>


using namespace boost::spirit::classic;
using namespace std;

BenchFileParser::BenchFileParser()
{
    cout << "[INFO] BenchFileParser created" << endl;
    m_netlist = new Netlist();
    parseFile("c17.bench");

}

/**
 * @brief this function is called by constructor to read the file header, count inputs, outputs, inverters and gates and store the numbers in member variables
 * @param file name
 */
void BenchFileParser::read_header(std::string inputFile)
{
    namespace qi = boost::spirit::qi;

    std::ifstream file(inputFile.c_str());
    std::string line;

    while (std::getline(file, line) && boost::starts_with(line, "#"))
    {
        std::vector<std::string> counter;

        //inputs section
        bool const i_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) ["#" >> +qi::digit >> "inputs"],
                counter);

        if (i_result && counter.size() == 1)
        {
            m_inputs_count = boost::lexical_cast<unsigned>(counter[0]);
        }

        //outputs section
        counter.clear();
        bool const o_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) ["#" >> +qi::digit >> "outputs"],
                counter);

        if (o_result && counter.size() == 1)
        {
            m_outputs_count = boost::lexical_cast<unsigned>(counter[0]);
        }

        //inverters section
        counter.clear();
        bool const inv_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) ["#" >> +qi::digit >> "inverter"],
                counter);

        if (inv_result && counter.size() == 1)
        {
            m_inverters_count = boost::lexical_cast<unsigned>(counter[0]);
        }

        //gates section
        counter.clear();
        bool const gate_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) ["#" >> +qi::digit >> "gates"],
                counter);
        if ( gate_result && counter.size() == 1)
        {
            read_gates(line);
        }
    }
}

/**
 * @brief function is called by read_header. It counts the number of ANDs, NANDs, ORs, NORs and buffers specified in the file header
 * @param line
 */
void BenchFileParser::read_gates(std::string line)
{
    namespace qi = boost::spirit::qi;

    //cout << "[DEBUG] reading gate line: " << line << endl;
    std::vector<std::string> counter;

    //AND sector
    bool const AND_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [qi::omit[qi::char_('#') >> +qi::digit >> "gates" >> qi::char_('(')]
            >> qi::omit[*(*qi::digit >> (qi::ascii::string("NANDs") | qi::ascii::string("ORs") | qi::ascii::string("NORs")| qi::ascii::string("buffers")) >> -qi::char_('+'))]
            >> *qi::digit >> "ANDs"
            >> qi::omit[*(-qi::char_('+') >> *qi::digit >> (qi::ascii::string("NANDs") | qi::ascii::string("ORs") | qi::ascii::string("NORs")| qi::ascii::string("buffers")) >> -qi::char_('+'))]
            >> qi::omit[qi::char_(')')]],
            counter);

    if (AND_result && counter.size() == 1)
    {
        m_ANDs_count = boost::lexical_cast<unsigned>(counter[0]);
        //cout << "    [DEBUG] m_ANDs:" << m_ANDs << endl;
    }

    //NAND sector
    counter.clear();
    bool const NAND_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [qi::omit[qi::char_('#') >> +qi::digit >> "gates" >> qi::char_('(')]
            >> qi::omit[*(*qi::digit >> (qi::ascii::string("ANDs") | qi::ascii::string("ORs") | qi::ascii::string("NORs")| qi::ascii::string("buffers")) >> -qi::char_('+'))]
            >> *qi::digit >> "NANDs"
            >> qi::omit[*(-qi::char_('+') >> *qi::digit >> (qi::ascii::string("ANDs") | qi::ascii::string("ORs") | qi::ascii::string("NORs")| qi::ascii::string("buffers")) >> -qi::char_('+'))]
            >> qi::omit[qi::char_(')')]],
            counter);

    if (NAND_result && counter.size() == 1)
    {
        m_NANDs_count = boost::lexical_cast<unsigned>(counter[0]);
        //cout << "    [DEBUG] m_NANDs:" << m_NANDs << endl;
    }

    //OR sector
    counter.clear();
    bool const OR_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [qi::omit[qi::char_('#') >> +qi::digit >> "gates" >> qi::char_('(')]
            >> qi::omit[*(*qi::digit >> (qi::ascii::string("ANDs") | qi::ascii::string("NANDs") | qi::ascii::string("NORs")| qi::ascii::string("buffers")) >> -qi::char_('+'))]
            >> *qi::digit >> "ORs"
            >> qi::omit[*(-qi::char_('+') >> *qi::digit >> (qi::ascii::string("ANDs") | qi::ascii::string("NANDs") | qi::ascii::string("NORs")| qi::ascii::string("buffers")) >> -qi::char_('+'))]
            >> qi::omit[qi::char_(')')]],
            counter);

    if (OR_result && counter.size() == 1)
    {
        m_ORs_count = boost::lexical_cast<unsigned>(counter[0]);
        //cout << "    [DEBUG] m_ORs:" << m_ORs << endl;
    }

    //NOR sector
    counter.clear();
    bool const NOR_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [qi::omit[qi::char_('#') >> +qi::digit >> "gates" >> qi::char_('(')]
            >> qi::omit[*(*qi::digit >> (qi::ascii::string("ANDs") | qi::ascii::string("NANDs") | qi::ascii::string("ORs")| qi::ascii::string("buffers")) >> -qi::char_('+'))]
            >> *qi::digit >> "NORs"
            >> qi::omit[*(-qi::char_('+') >> *qi::digit >> (qi::ascii::string("ANDs") | qi::ascii::string("NANDs") | qi::ascii::string("ORs")| qi::ascii::string("buffers")) >> -qi::char_('+'))]
            >> qi::omit[qi::char_(')')]],
            counter);

    if (NOR_result && counter.size() == 1)
    {
        m_NORs_count = boost::lexical_cast<unsigned>(counter[0]);
        //cout << "    [DEBUG] m_NORs:" << m_NORs << endl;
    }

    //buffer sector
    counter.clear();
    bool const buffer_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [qi::omit[qi::char_('#') >> +qi::digit >> "gates" >> qi::char_('(')]
            >> qi::omit[*(*qi::digit >> (qi::ascii::string("ANDs") | qi::ascii::string("NANDs") | qi::ascii::string("ORs")| qi::ascii::string("NORs")) >> -qi::char_('+'))]
            >> *qi::digit >> "buffers"
            >> qi::omit[*(-qi::char_('+') >> *qi::digit >> (qi::ascii::string("ANDs") | qi::ascii::string("NANDs") | qi::ascii::string("ORs")| qi::ascii::string("NORs")) >> -qi::char_('+'))]
            >> qi::omit[qi::char_(')')]],
            counter);

    if (buffer_result && counter.size() == 1)
    {
        m_buffers_count = boost::lexical_cast<unsigned>(counter[0]);
        //cout << "    [DEBUG] m_bufers:" << m_buffers << endl;
    }
}
//TODO: accept mixed type instead of number
//TODO: accept more than two inputs

void BenchFileParser::read_body(string inputFile)
{
    std::cout << "[INFO] reading body" << std::endl;

    namespace qi = boost::spirit::qi;

    std::ifstream file(inputFile.c_str());
    std::string line;
    while (std::getline(file, line))
    {
        if (boost::starts_with(line, "#")) {
            continue;
        }

//        cout << "[DEBUG] reading line: " << line << endl;
        std::vector<std::string> values_str;

        //inputs section
        bool const i_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) [qi::omit["INPUT" >> qi::char_("(")] >> *qi::digit >> qi::omit[qi::char_(")")]],
                values_str);

        if (i_result && values_str.size() == 1)
        {
            m_netlist->addPrimaryInput(new Signal(values_str[0], true));
        }

        //outputs section
        values_str.clear();
        bool const o_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) [qi::omit["OUTPUT" >> qi::char_("(")] >> *qi::digit >> qi::omit[qi::char_(")")]],
                values_str);
//        cout << "[DEBUG] o_result: " << std::boolalpha << o_result << endl;
        if (o_result && values_str.size() == 1)
        {
            m_netlist->addPrimaryOutput(new Signal(values_str[0], true));
        }

        //AND section
        values_str.clear();
        bool const and_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) [*qi::digit >> qi::omit[qi::char_("=") >> "AND" >> qi::char_("(")] >> *qi::digit % qi::char_(",") >> qi::omit[qi::char_(")")]],
                values_str);
//        cout << "[DEBUG] and_result: " << std::boolalpha << and_result << endl;
        if (and_result)
        {
            Signal* so = m_netlist->primaryOutputByName(values_str[0]);
            if (!so)
            {
                so = new Signal(values_str[0]);
            }
            vector<Signal*> inputs;
            BOOST_FOREACH(std::string signalName, values_str)
            {
                if (signalName == values_str[0]){
                    continue;
                }
                Signal* s = m_netlist->primaryInputByName(signalName);
                if (s)
                {
                    inputs.push_back(s);
                } else {
                    inputs.push_back(new Signal(signalName));
                }
            }

            m_netlist->addAND(new AND(inputs,so));
        }

        //NAND section
        values_str.clear();
        bool const nand_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) [*qi::digit >> qi::omit[qi::char_("=") >> "NAND" >> qi::char_("(")] >> *qi::digit % qi::char_(",") >> qi::omit[qi::char_(")")]],
                values_str);
//        cout << "[DEBUG] nand_result: " << std::boolalpha << nand_result << endl;
        if (nand_result)
        {
            Signal* so = m_netlist->primaryOutputByName(values_str[0]);
            if (!so)
            {
                so = new Signal(values_str[0]);
            }
            vector<Signal*> inputs;
            BOOST_FOREACH(std::string signalName, values_str)
            {
                if (signalName == values_str[0]){
                    continue;
                }
                Signal* s = m_netlist->primaryInputByName(signalName);
                if (s)
                {
                    inputs.push_back(s);
                } else {
                    inputs.push_back(new Signal(signalName));
                }
            }

            m_netlist->addNAND(new NAND(inputs,so));
        }

        //OR section
        values_str.clear();
        bool const or_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) [*qi::digit >> qi::omit[qi::char_("=") >> "OR" >> qi::char_("(")] >> *qi::digit % qi::char_(",") >> qi::omit[qi::char_(")")]],
                values_str);
//        cout << "[DEBUG] or_result: " << std::boolalpha << or_result << endl;
        if (or_result)
        {
            Signal* so = m_netlist->primaryOutputByName(values_str[0]);
            if (!so)
            {
                so = new Signal(values_str[0]);
            }
            vector<Signal*> inputs;
            BOOST_FOREACH(std::string signalName, values_str)
            {
                if (signalName == values_str[0]){
                    continue;
                }
                Signal* s = m_netlist->primaryInputByName(signalName);
                if (s)
                {
                    inputs.push_back(s);
                } else {
                    inputs.push_back(new Signal(signalName));
                }
            }

            m_netlist->addOR(new OR(inputs,so));
        }

        //NOR section
        values_str.clear();
        bool const nor_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) [*qi::digit >> qi::omit[qi::char_("=") >> "NOR" >> qi::char_("(")] >> *qi::digit % qi::char_(",") >> qi::omit[qi::char_(")")]],
                values_str);
//        cout << "[DEBUG] nor_result: " << std::boolalpha << nor_result << endl;
        if (nor_result)
        {
            Signal* so = m_netlist->primaryOutputByName(values_str[0]);
            if (!so)
            {
                so = new Signal(values_str[0]);
            }
            vector<Signal*> inputs;
            BOOST_FOREACH(std::string signalName, values_str)
            {
                if (signalName == values_str[0]){
                    continue;
                }
                Signal* s = m_netlist->primaryInputByName(signalName);
                if (s)
                {
                    inputs.push_back(s);
                } else {
                    inputs.push_back(new Signal(signalName));
                }
            }

            m_netlist->addNOR(new NOR(inputs,so));
        }

        //NOT section
        values_str.clear();
        bool const not_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) [*qi::digit >> qi::omit[qi::char_("=") >> "NOT" >> qi::char_("(")] >> *qi::digit >> qi::omit[qi::char_(",")] >> *qi::digit >> qi::omit[qi::char_(")")]],
                values_str);
//        cout << "[DEBUG] not_result: " << std::boolalpha << not_result << endl;
        if (not_result && values_str.size() == 2)
        {
            Signal* so = m_netlist->primaryOutputByName(values_str[0]);
            if (!so)
            {
                so = new Signal(values_str[0]);
            }
            vector<Signal*> inputs;
            Signal* si = m_netlist->primaryInputByName(values_str[1]);
            if (!si)
            {
                si = new Signal(values_str[1]);
            }
            inputs.push_back(si);

            m_netlist->addNOT(new NOT(inputs,so));
        }

        //BUF section
        values_str.clear();
        bool const buf_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) [*qi::digit >> qi::omit[qi::char_("=") >> "BUF" >> qi::char_("(")] >> *qi::digit >> qi::omit[qi::char_(",")] >> *qi::digit >> qi::omit[qi::char_(")")]],
                values_str);
//        cout << "[DEBUG] buf_result: " << std::boolalpha << buf_result << endl;
        if (buf_result && values_str.size() == 2)
        {
            Signal* so = m_netlist->primaryOutputByName(values_str[0]);
            if (!so)
            {
                so = new Signal(values_str[0]);
            }
            vector<Signal*> inputs;
            Signal* si = m_netlist->primaryInputByName(values_str[1]);
            if (!si)
            {
                si = new Signal(values_str[1]);
            }
            inputs.push_back(si);

            m_netlist->addBUF(new BUF(inputs,so));
        }
    }
}

/**
 * @brief parses file specified by inputFile
 * @param inputFile file name
 */
void BenchFileParser::parseFile(std::string inputFile)
{
    cout << "[INFO] try to parse " << inputFile.c_str() << endl;
    cout << "[INFO] reading header" << endl;
//    read_header(inputFile);
    read_body(inputFile);
    prettyPrintInfos();
    cout << "[INFO] end parsing " << inputFile.c_str() << endl;
}

void BenchFileParser::prettyPrintInfos()
{
    cout << "[STAT] " << endl;
    cout << "       inputs:    " << m_inputs_count << endl;
    cout << "       outputs:   " << m_outputs_count << endl;
    cout << "       inverters: " << m_inverters_count << endl;
    cout << "       AND Gates: " << m_ANDs_count << endl;
    cout << "       OR Gates:  " << m_ORs_count << endl;
    cout << "       NAND Gates:" << m_NANDs_count << endl;
    cout << "       NOR Gates: " << m_NORs_count << endl;
    cout << "       buffers:   " << m_buffers_count << endl;

    if (m_netlist->primaryInputs().size() >= 1){
        cout << "       inputs: ";
        BOOST_FOREACH(Signal* s, m_netlist->primaryInputs())
        {
            cout << s->name() << " ";
        }
        cout << endl;
    }
    if (m_netlist->primaryOutputs().size() >= 1){
        cout << "       outputs: ";
        BOOST_FOREACH(Signal* s, m_netlist->primaryOutputs())
        {
            cout << s->name() << " ";
        }
        cout << endl;
    }
    if (m_netlist->ANDs().size() >= 1)
    {
        cout << "[STAT] ANDs:" << endl;
        BOOST_FOREACH(AND* n, m_netlist->ANDs())
        {
            n->prettyPrint();
        }
    }
    if (m_netlist->NANDs().size() >= 1)
    {
        cout << "[STAT] NANDs:" << endl;
        BOOST_FOREACH(NAND* n, m_netlist->NANDs())
        {
            n->prettyPrint();
        }
    }
}
