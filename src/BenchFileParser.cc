#include "BenchFileParser.h"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/home/classic/actor.hpp>
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_match.hpp>
#include <boost/spirit/version.hpp>


using namespace boost::spirit::classic;
using namespace std;

BenchFileParser::BenchFileParser()
{
    cout << "[INFO] BenchFileParser created" << endl;
    parseFile("c17.bench");

}

void BenchFileParser::parseFile(std::string inputFile)
{
    cout << "[INFO] try to parse " << inputFile.c_str() << endl;

    std::ifstream file(inputFile.c_str());
    std::string line;
    unsigned input_counter = 0;

    while (std::getline(file, line))
    {
        cout << "[DEBUG] " << line << endl;
        parse(line.c_str(),
              *( boost::spirit::classic::str_p("INPUT")[increment_a(input_counter)]
              | boost::spirit::classic::anychar_p
              ));
        cout << "[DEBUG] input_counter: " << input_counter << endl;
    }

    cout << "[DEBUG] file contains " << input_counter << " inputs" << endl;
    cout << "[INFO] end parsing " << inputFile.c_str() << endl;
}

