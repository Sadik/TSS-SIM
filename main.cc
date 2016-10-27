#include <algorithm>
#include <iostream>
#include <iterator>

#include "src/BenchFileParser.h"

#include <boost/foreach.hpp>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2 || argc > 3)
    {
        cout << "wrong arguments" << endl;
        exit(-1);
    }
    auto parser = new BenchFileParser(argv[1]);

   /* BOOST_FOREACH( char ch, hello )
    {
        std::cout << ch << endl;
    }
*/
    cout << "[INFO] Beende Programm" << endl;
    return 0;
}

