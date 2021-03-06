/**
 * @file   BenchFileParser.cc
 * @Author Sadik Özoguz (sadik@informatik.uni-bremen.de)
 * @date   Oktober, 2016
 * @brief  Parses a bench file hopefully
 *
 * Some time it will...
 */

#include "BenchFileParser.h"

#include <algorithm>

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

#include <boost/spirit/home/classic/actor.hpp>
#include <boost/spirit/include/classic.hpp>
#include <boost/spirit/include/qi_char_.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/qi_match.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/version.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>

using namespace boost;
using namespace boost::spirit::classic;
namespace qi = boost::spirit::qi;

BenchFileParser::BenchFileParser(const std::string benchFile, Netlist* netlist)
{
    std::cout << "[INFO] BenchFileParser created" << std::endl;
    parseBenchFile(benchFile, netlist);
}

/**
 * @brief BenchFileParser::parsePrims
 * store primary inputs and primary outputs in m_netlist->m_primaryInputs and m_netlist->m_primaryOutputs
 *
 * @param line
 */
void BenchFileParser::parsePrims(const std::string& line, Netlist* netlist)
{
    std::vector<std::string> values_str;

    //inputs section
    bool const i_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [qi::omit["INPUT" >> qi::char_("(")] >> *qi::alnum >> qi::omit[qi::char_(")")]],
            values_str);

    if (i_result && values_str.size() == 1)
    {
        netlist->addPrimaryInput(boost::make_shared<Signal>(values_str[0], true));
    }

    //outputs section
    values_str.clear();
    bool const o_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [qi::omit["OUTPUT" >> qi::char_("(")] >> *qi::alnum >> qi::omit[qi::char_(")")]],
            values_str);
//        cout << "[DEBUG] o_result: " << std::boolalpha << o_result << endl;
    if (o_result && values_str.size() == 1)
    {
        netlist->addPrimaryOutput(boost::make_shared<Signal>(values_str[0], true));
    }
}

/**
 * @brief BenchFileParser::parseANDs
 * checks if line specifies an AND gate.
 * stores the AND gate with it's inputs and outputs.
 * Also has a bit optimization here: If the AND's input is primaryInput or primaryOutput, store this primary signal instead of createing a new one.
 * This assumes that primary inputs and outputs have already been read and stored. We don't have this assumption for any other type.
 *
 * @param line
 */
void BenchFileParser::parseANDs(const std::string& line, Netlist* netlist)
{
    std::vector<std::string> values_str;
    //AND section
    values_str.clear();
    bool const and_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [*qi::alnum >> qi::omit[qi::char_("=") >> "AND" >> qi::char_("(")] >> *qi::alnum % qi::char_(",") >> qi::omit[qi::char_(")")]],
            values_str);
//        cout << "[DEBUG] and_result: " << std::boolalpha << and_result << endl;
    if (and_result)
    {
        shared_ptr<Signal> so = netlist->signalByName(values_str[0]);
        if (!so)
        {
            so = boost::make_shared<Signal>(values_str[0]);
        }
        std::vector< shared_ptr<Signal> > inputs;
        BOOST_FOREACH(std::string signalName, values_str)
        {
            //the first value stored in values_str is the output signal.
            if (signalName == values_str[0]){
                continue;
            }
            boost::shared_ptr<Signal> s = netlist->signalByName(signalName);
            if (s)
            {
                inputs.push_back(s);
            } else {
                inputs.push_back(boost::make_shared<Signal>(signalName));
            }
        }

        shared_ptr<AND> newAnd = boost::make_shared<AND>(inputs, so);
        netlist->addAND(newAnd);
        BOOST_FOREACH(auto s, inputs)
        {
            s->setTarget(newAnd);
        }
        so->setSource(newAnd);
    }
}

/**
 * @brief BenchFileParser::parseNANDs
 * equivalent to parseANDs
 * @param line
 */
void BenchFileParser::parseNANDs(const std::string& line, Netlist* netlist)
{
    std::vector<std::string> values_str;

    //NAND section
    values_str.clear();
    bool const nand_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [*qi::alnum >> qi::omit[qi::char_("=") >> "NAND" >> qi::char_("(")] >> *qi::alnum % qi::char_(",") >> qi::omit[qi::char_(")")]],
            values_str);
//        cout << "[DEBUG] nand_result: " << std::boolalpha << nand_result << endl;
    if (nand_result)
    {
//        Signal* output = netlist->primaryOutputByName(values_str[0]);
        shared_ptr<Signal> output = netlist->signalByName(values_str[0]);
        if (!output)
        {
            output = boost::make_shared<Signal>(values_str[0]);
        }

        shared_ptr<NAND> newNand = boost::make_shared<NAND>();
        newNand->setOutput(output);
        output->setSource(newNand);
        netlist->addSignal(output);
        netlist->addNAND(newNand);
        BOOST_FOREACH(std::string signalName, values_str)
        {
            if (signalName == values_str[0]){
                continue;
            }
            shared_ptr<Signal> s = netlist->signalByName(signalName);
//            Signal* s = netlist->primaryInputByName(signalName);
            if (!s)
            {
                s = boost::make_shared<Signal>(signalName);
            }
            if (s->hasTarget())
            {
                // need to create a fanout
                shared_ptr<Fanout> fanout = boost::make_shared<Fanout>();
                fanout->setInput(s);
                shared_ptr<Signal> afterFanout1 = boost::make_shared<Signal>(s->name() + ".1_" + std::to_string(netlist->NANDs().size()));
                shared_ptr<Signal> afterFanout2 = boost::make_shared<Signal>(s->name() + ".2_" + std::to_string(netlist->NANDs().size()));
                afterFanout1->setSource(fanout);
                afterFanout2->setSource(fanout);
                netlist->addSignal(afterFanout1);
                netlist->addSignal(afterFanout2);

                auto gate1 = s->target();
                afterFanout1->setTarget(gate1);
                gate1->replaceInput(s, afterFanout1);

                s->setTarget(fanout);
                afterFanout2->setTarget(newNand);
                newNand->addInput(afterFanout2);
            } else {
                netlist->addSignal(s);
                s->setTarget(newNand);
                newNand->addInput(s);
            }
        }
    }
}

/**
 * @brief BenchFileParser::parseORs
 * equivalent to parseANDs
 * @param line
 */
void BenchFileParser::parseORs(const std::string& line, Netlist* netlist)
{
    std::vector<std::string> values_str;
    //OR section
    values_str.clear();
    bool const or_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [*qi::alnum >> qi::omit[qi::char_("=") >> "OR" >> qi::char_("(")] >> *qi::alnum % qi::char_(",") >> qi::omit[qi::char_(")")]],
            values_str);
//        cout << "[DEBUG] or_result: " << std::boolalpha << or_result << endl;
    if (or_result)
    {
        shared_ptr<Signal>  so = netlist->signalByName(values_str[0]);
        if (!so)
        {
            so = boost::make_shared<Signal>(values_str[0]);
        }
        std::vector< boost::shared_ptr<Signal> > inputs;
        BOOST_FOREACH(std::string signalName, values_str)
        {
            if (signalName == values_str[0]){
                continue;
            }
            shared_ptr<Signal> s = netlist->signalByName(signalName);
            if (s)
            {
                inputs.push_back(s);
            } else {
                inputs.push_back(boost::make_shared<Signal>(signalName));
            }
        }

        shared_ptr<OR> newOr = boost::make_shared<OR>(inputs, so);
        netlist->addOR(newOr);
        BOOST_FOREACH(auto s, inputs)
        {
            s->setTarget(newOr);
        }
        so->setSource(newOr);
    }
}

/**
 * @brief BenchFileParser::parseNORs
 * equivalent to parseANDs
 * @param line
 */
void BenchFileParser::parseNORs(const std::string& line, Netlist* netlist)
{
    std::vector<std::string> values_str;
    //NOR section
    values_str.clear();
    bool const nor_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [*qi::alnum >> qi::omit[qi::char_("=") >> "NOR" >> qi::char_("(")] >> *qi::alnum % qi::char_(",") >> qi::omit[qi::char_(")")]],
            values_str);
//        cout << "[DEBUG] nor_result: " << std::boolalpha << nor_result << endl;
    if (nor_result)
    {
        shared_ptr<Signal> so = netlist->signalByName(values_str[0]);
        if (!so)
        {
            so = boost::make_shared<Signal>(values_str[0]);
        }
        std::vector< boost::shared_ptr<Signal> > inputs;
        BOOST_FOREACH(std::string signalName, values_str)
        {
            if (signalName == values_str[0]){
                continue;
            }
            shared_ptr<Signal>  s = netlist->signalByName(signalName);
            if (s)
            {
                inputs.push_back(s);
            } else {
                inputs.push_back(boost::make_shared<Signal>(signalName));
            }
        }

        shared_ptr<NOR> newNor = boost::make_shared<NOR>(inputs, so);
        netlist->addNOR(newNor);
        BOOST_FOREACH(auto s, inputs)
        {
            s->setTarget(newNor);
        }
        so->setSource(newNor);
    }
}

/**
 * @brief BenchFileParser::parseNOTs
 * equivalent to parseANDs
 * @param line
 */
void BenchFileParser::parseNOTs(const std::string& line, Netlist* netlist)
{
    std::vector<std::string> values_str;
    //NOT section
    values_str.clear();
    bool const not_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [*qi::alnum >> qi::omit[qi::char_("=") >> "NOT" >> qi::char_("(")] >> *qi::alnum >> qi::omit[qi::char_(")")]],
            values_str);
//        cout << "[DEBUG] not_result: " << std::boolalpha << not_result << endl;
    if (not_result && values_str.size() == 2)
    {
        shared_ptr<Signal> so = netlist->signalByName(values_str[0]);
        if (!so)
        {
            so = boost::make_shared<Signal>(values_str[0]);
        }
        std::vector< shared_ptr<Signal> > inputs;
        boost::shared_ptr<Signal> si = netlist->signalByName(values_str[1]);
        if (!si)
        {
            si = boost::make_shared<Signal>(values_str[1]);
        }
        inputs.push_back(si);

        shared_ptr<NOT> newNot = boost::make_shared<NOT>(inputs, so);
        netlist->addNOT(newNot);
        BOOST_FOREACH(auto s, inputs)
        {
            s->setTarget(newNot);
        }
        so->setSource(newNot);
    }
}

/**
 * @brief BenchFileParser::parseBUFs
 * equivalent to parseANDs
 * @param line
 */
void BenchFileParser::parseBUFs(const std::string& line, Netlist* netlist)
{
    std::vector<std::string> values_str;
    //BUF section
    values_str.clear();
    bool const buf_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [*qi::alnum >> qi::omit[qi::char_("=") >> "BUF" >> qi::char_("(")] >> *qi::alnum >> qi::omit[qi::char_(")")]],
            values_str);
//        cout << "[DEBUG] buf_result: " << std::boolalpha << buf_result << endl;
    if (buf_result && values_str.size() == 2)
    {
        shared_ptr<Signal> so = netlist->signalByName(values_str[0]);
        if (!so)
        {
            so = boost::make_shared<Signal>(values_str[0]);
        }
        std::vector< shared_ptr<Signal> > inputs;
        shared_ptr<Signal> si = netlist->signalByName(values_str[1]);
        if (!si)
        {
            si = boost::make_shared<Signal>(values_str[1]);
        }
        inputs.push_back(si);

        shared_ptr<BUF> newBuf = boost::make_shared<BUF>(inputs, so);
        netlist->addBUF(newBuf);
        BOOST_FOREACH(auto s, inputs)
        {
            s->setTarget(newBuf);
        }
        so->setSource(newBuf);
    }
}

void BenchFileParser::parseDFFs(const std::string& line, Netlist* netlist)
{
    std::vector<std::string> values_str;
    //DFF section
    values_str.clear();
    //cout << "IS THIS A DFF? " << line << endl;
    bool const dff_result = qi::parse(line.begin(), line.end(),
            qi::skip(qi::space) [*qi::alnum >> qi::omit[qi::char_("=") >> "DFF" >> qi::char_("(")] >> *qi::alnum >> qi::omit[qi::char_(")")]],
            values_str);
        //cout << "[DEBUG] dff_result: " << std::boolalpha << dff_result << endl;
    if (dff_result && values_str.size() == 2)
    {
        //we want to make [0] a primary output and [1] a primary input.
        //simplification
        //cout << "[DEBUG] [0]: " << values_str[0] << endl;
        // cout << "[DEBUG] [1]: " << values_str[1] << endl;
        shared_ptr<Signal> so = netlist->primaryOutputByName(values_str[1]);
        if(!so)
        {
            netlist->addPrimaryOutput(boost::make_shared<Signal>(values_str[1], true));
        }

        so = netlist->primaryInputByName(values_str[0]);
        if(!so)
        {
            netlist->addPrimaryInput(boost::make_shared<Signal>(values_str[0], true));
        }
    }
}

/**
 * @brief BenchFileParser::read_body
 * Skip comments (line starts with'#')
 * read bench file specified by inputFile and store pimary inputs, primary outputs, DFFs and all gates into the netlist data structure (class Netlist)
 * The assumption here is that the bench file format specifies all primary inputs and primary outputs first before specifying Gates or Flip Flops.
 * By reading the .bench files I can't see that this assumption is incorrect
 *
 * @param inputFile file in bench format
 */
void BenchFileParser::parseBenchFile(const std::string &inputFile, Netlist* netlist)
{
    std::cout << "[INFO] reading bench file" << std::endl;

    std::ifstream file(inputFile.c_str());
    std::string line;
    while (std::getline(file, line))
    {
        if (boost::starts_with(line, "#")) {
            continue;
        }
//        cout << "[DEBUG] reading line: " << line << endl;
        parsePrims(line, netlist);
        parseANDs(line, netlist);
        parseNANDs(line, netlist);
        parseORs(line, netlist);
        parseNORs(line, netlist);
        parseNOTs(line, netlist);
        parseBUFs(line, netlist);
        parseDFFs(line, netlist);
    }
}
