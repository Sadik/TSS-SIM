#include <algorithm>
#include <iostream>
#include <iterator>

#include "src/BenchFileParser.h"
#include "src/PatternFileParser.h"

#include <boost/foreach.hpp>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3 || argc > 3)
    {
        cout << "usage: TSS-SIM bench-file pattern-file" << endl;
        exit(-1);
    }

    Netlist* netlist = new Netlist();
    BenchFileParser* benchFileParser = new BenchFileParser(argv[1], netlist);
    PatternFileParser* patternFileParser = new PatternFileParser(argv[2]);
    std::vector<boost::dynamic_bitset<>> testPattern = patternFileParser->getTestPattern();
    cout << "[INFO] tests: " << testPattern.size() << endl;

    netlist->prepare();
    std::cout << "[INFO] stuck-at faults:         " << 2 * netlist->getAllSignals().size() << std::endl;
    std::cout << "[INFO] after trivial reduction: " << netlist->getAllFaults().size() << std::endl;

    netlist->startSimulation(testPattern);

   /* BOOST_FOREACH( char ch, hello )
    {
        std::cout << ch << endl;
    }
*/
    cout << "[INFO] Beende Programm" << endl;
    return 0;
}

