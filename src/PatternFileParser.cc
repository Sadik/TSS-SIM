#include "PatternFileParser.h"
#include <fstream>

#include <boost/algorithm/string.hpp>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;

PatternFileParser::PatternFileParser(const std::string& fileName)
{
    readPatternFile(fileName);
}

void PatternFileParser::readPatternFile(const std::string& patternFile)
{
    cout << "[INFO] try to read " << patternFile.c_str() << endl;
    std::ifstream file(patternFile.c_str());
    std::string line;

    while (std::getline(file, line))
    {
        if (boost::starts_with(line, "#")){
            continue;
        }

        std::string testCase;

        bool const patternFound = qi::parse(line.begin(), line.end(),
                *qi::digit,
                testCase);
        if (patternFound)
        {
            std::reverse(testCase.begin(), testCase.end());
            boost::dynamic_bitset<> bits(testCase);
            m_testPattern.push_back(bits);
        }
    }
    cout << "[INFO] end reading " << patternFile.c_str() << endl;
}

std::vector<boost::dynamic_bitset<> > PatternFileParser::getTestPattern()
{
    return m_testPattern;
}
