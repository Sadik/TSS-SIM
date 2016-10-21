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

unsigned BenchFileParser::count_inputs(std::string inputFile)
{
    unsigned input_counter = 0;
    std::ifstream file(inputFile.c_str());
    std::string line;

    while (std::getline(file, line))
    {
        cout << "[DEBUG] " << line << endl;
        parse(line.c_str(),
              *( str_p("inputs")[increment_a(input_counter)]
              | anychar_p
              ));
        cout << "[DEBUG] input_counter: " << input_counter << endl;
    }

    return input_counter;
}

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
            m_inverter = boost::lexical_cast<unsigned>(counter[0]);
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

void BenchFileParser::parseFile(std::string inputFile)
{
    cout << "[INFO] try to parse " << inputFile.c_str() << endl;
    cout << "[INFO] reading header" << endl;
    read_header(inputFile);
    cout << "[INFO] end parsing " << inputFile.c_str() << endl;
}

