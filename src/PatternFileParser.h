#pragma once

#include <iostream>
#include <string>

#include <boost/dynamic_bitset.hpp>

using namespace std;

class PatternFileParser
{
public:
    PatternFileParser(const string &fileName);
    std::vector<boost::dynamic_bitset<>> getTestPattern();

private:
    std::vector<boost::dynamic_bitset<> > m_testPattern;

private:
    void readPatternFile(const std::string& patternFile);
};
