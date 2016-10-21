#include <algorithm>
#include <iostream>
#include <iterator>

#include "src/BenchFileParser.h"

#include <boost/foreach.hpp>

using namespace std;

int main(int argc, char** argv)
{
    auto parser = new BenchFileParser();

   /* BOOST_FOREACH( char ch, hello )
    {
        std::cout << ch << endl;
    }
*/
    cout << "[INFO] Beende Programm" << endl;
    return 0;
}

