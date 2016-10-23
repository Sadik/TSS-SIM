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
            m_inputs = boost::lexical_cast<unsigned>(counter[0]);
        }

        //outputs section
        counter.clear();
        bool const o_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) ["#" >> +qi::digit >> "outputs"],
                counter);

        if (o_result && counter.size() == 1)
        {
            m_outputs = boost::lexical_cast<unsigned>(counter[0]);
        }

        //inverters section
        counter.clear();
        bool const inv_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) ["#" >> +qi::digit >> "inverter"],
                counter);

        if (inv_result && counter.size() == 1)
        {
            m_inverters = boost::lexical_cast<unsigned>(counter[0]);
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
        m_ANDs = boost::lexical_cast<unsigned>(counter[0]);
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
        m_NANDs = boost::lexical_cast<unsigned>(counter[0]);
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
        m_ORs = boost::lexical_cast<unsigned>(counter[0]);
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
        m_NORs = boost::lexical_cast<unsigned>(counter[0]);
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
        m_buffers = boost::lexical_cast<unsigned>(counter[0]);
        //cout << "    [DEBUG] m_bufers:" << m_buffers << endl;
    }
}

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
            m_i_values.push_back( boost::lexical_cast<unsigned>(values_str[0]) );
        }

        //outputs section
        values_str.clear();
        bool const o_result = qi::parse(line.begin(), line.end(),
                qi::skip(qi::space) [qi::omit["OUTPUT" >> qi::char_("(")] >> *qi::digit >> qi::omit[qi::char_(")")]],
                values_str);
//        cout << "[DEBUG] o_result: " << std::boolalpha << o_result << endl;
        if (o_result && values_str.size() == 1)
        {
            m_o_values.push_back( boost::lexical_cast<unsigned>(values_str[0]) );
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
    read_header(inputFile);
    read_body(inputFile);
    prettyPrintInfos();
    cout << "[INFO] end parsing " << inputFile.c_str() << endl;
}

void BenchFileParser::prettyPrintInfos()
{
    cout << "[STAT] " << endl;
    cout << "       inputs:    " << m_inputs << endl;
    cout << "       outputs:   " << m_outputs << endl;
    cout << "       inverters: " << m_inverters << endl;
    cout << "       AND Gates: " << m_ANDs << endl;
    cout << "       OR Gates:  " << m_ORs << endl;
    cout << "       NAND Gates:" << m_NANDs << endl;
    cout << "       NOR Gates: " << m_NORs << endl;
    cout << "       buffers:   " << m_buffers << endl;

    if (m_i_values.size() >= 1){
        cout << "       input values: ";
        BOOST_FOREACH(unsigned u, m_i_values)
        {
            cout << u << " ";
        }
        cout << endl;
    }
    if (m_o_values.size() >= 1){
        cout << "       output values: ";
        BOOST_FOREACH(unsigned u, m_o_values)
        {
            cout << u << " ";
        }
        cout << endl;
    }
}
