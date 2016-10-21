#include "BenchFileParser.h"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/home/classic/actor.hpp>
#include <boost/spirit/include/classic.hpp>
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
    using boost::spirit::qi::skip;

    std::ifstream file(inputFile.c_str());
    std::string line;

    while (std::getline(file, line) && boost::starts_with(line, "#"))
    {
        cout << "[DEBUG] " << line << endl;
        std::vector<std::string> counter;

        //inputs section
        bool const i_result = qi::parse(line.begin(), line.end(),
                  skip(qi::space) ["#" >> +qi::digit >> "inputs"],
                  counter);

        if (i_result && counter.size() == 1)
        {
            m_inputs = boost::lexical_cast<unsigned>(counter[0]);
        }

        //outputs section
        counter.clear();
        bool const o_result = qi::parse(line.begin(), line.end(),
                  skip(qi::space) ["#" >> +qi::digit >> "outputs"],
                  counter);

        if (o_result && counter.size() == 1)
        {
            m_outputs = boost::lexical_cast<unsigned>(counter[0]);
        }

        //inverters section
        counter.clear();
        bool const inv_result = qi::parse(line.begin(), line.end(),
                  skip(qi::space) ["#" >> +qi::digit >> "inverter"],
                  counter);

        if (inv_result && counter.size() == 1)
        {
            m_inverter = boost::lexical_cast<unsigned>(counter[0]);
        }
    }
}

void BenchFileParser::parseFile(std::string inputFile)
{
    cout << "[INFO] try to parse " << inputFile.c_str() << endl;
    cout << "[INFO] reading header" << endl;
    read_header(inputFile);
    cout << "[INFO] end parsing " << inputFile.c_str() << endl;
}

