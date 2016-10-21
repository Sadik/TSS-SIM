#pragma once

#include <iostream>
#include <string>
#include <fstream>

class BenchFileParser
{
public:
    BenchFileParser();
private:
    void parseFile(std::string inputFile);
};
