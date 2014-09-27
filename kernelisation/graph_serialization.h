#include "types.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <vector>
#include <iostream>
#include <fstream>

reductionTypes::BaseGraph buildGraph(int nverticies, std::vector<std::pair<int, int>> edges) {
    reductionTypes::BaseGraph g(nverticies);
    for (auto e : edges) {
        boost::add_edge(e.first, e.second, g);
    }
    return g;
}

template<typename T>
void outputGraph(std::ostream &fout, const T &g) {
    //Print graph to stdout, unlabelled
    boost::write_graphviz(fout, g);

}

reductionTypes::BaseGraph readGraph(std::istream &fin) {
    reductionTypes::BaseGraph g(0);
    boost::dynamic_properties dp(ignore_other_properties);
    boost::read_graphviz(fin, g, dp);
    return g;

}