#include <iostream>
#include <vector>

#include <boost/graph/graphviz.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>

#include <boost/program_options.hpp>

#include "kernelisation/graph_serialization.h"
#include "kernelisation/graph_reduction.h"

#include "brute.h"
#include "branch.h"

using namespace boost;
using namespace std;

namespace po = boost::program_options;

int main(int argc ,char* argv[])
{

    po::options_description  desc("Options");

    desc.add_options()
            ("help,h", "Print help messages")
            ("brute,b","brute force")
            ("branch,s","Branch without kernelisation")
            ("kbranch,r","Branch with kernelisation")
            ("max-k,k", po::value<int>()->implicit_value(1),"Max-k")
            ("output-file,o", po::value<vector<string>>(), "Specifies output file.")
            ("input-file,i", po::value<vector<string>>(),"Specifies input file");
    po::positional_options_description p;
    p.add("input-file", -1);

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(),vm);
        if (vm.count("help")){
            cout << "Programming Assessment task 2" << endl;
            cout << "Check G5 Graph Domination" << endl;
            cout << "  Alexis Shaw z3372395" << endl;
            cout << "  Mitch Ward" << endl;
            cout << "  Sindre Fjermestad" << endl;
            cout << desc << endl;

            return 0;
        }
        po::notify(vm);
    } catch(po::error& e) {
        cerr << "ERROR: " << e.what() << endl << endl;
        cerr << desc << endl;
    }

    ifstream inputFile;
    istream* pCin = &cin;

    if(vm.count("input-file")){
        vector<string> iFileName = vm["input-file"].as<vector<string>>();
        if (iFileName[0].compare("-") != 0) {
            inputFile.open(iFileName[0].c_str(), ifstream::in);
            pCin = &inputFile;
        }
    }

    ofstream outputfile;
    ostream* pCout = &cout;

    if(vm.count("output-file")){
        vector<string> oFileName = vm["input-file"].as<vector<string>>();
        if (oFileName[0].compare("-") != 0) {
            outputfile.open(oFileName[0].c_str(), ifstream::in);
            pCout = &outputfile;
        }
    }

    int k = vm["max-k"].as<int>();



    //ofstream myfile;
    //myfile.open("graph.dot");
    //outputGraph(buildGraph(n, edges));
    //ifstream fin("graph.dot");
    BaseGraph gBase = readGraph(*pCin);

    Graph g(gBase);
    vector<GraphModifier> myvect(1000);
    stack<GraphModifier, vector<GraphModifier>> kernState(myvect);

    GraphReduction::kernelize(g, k, kernState);

    *pCout << g[graph_bundle].numBlack << " " << g[graph_bundle].numRed<<endl;
    if(vm.count("brute")){
        *pCout << brute(g, k) << endl;
    }
    if(vm.count("branch")){
        vector<stack<GraphModifier, vector<GraphModifier>>> stacks(k);
        *pCout << branch(g, k, false, stacks) << endl;
    }
    if(vm.count("kbranch")){
        vector<stack<GraphModifier, vector<GraphModifier>>> stacks(k);
        *pCout << branch(g, k, true, stacks) << endl;
    }

    *pCout << g[graph_bundle].numBlack << " " << g[graph_bundle].numRed<<endl;


    //outputGraph(*pCout, g);

}