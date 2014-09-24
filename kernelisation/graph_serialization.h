#include "types.h"
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
using namespace boost;


BaseGraph buildGraph(int nverticies, vector<pair<int, int>> edges) {
    BaseGraph g(nverticies);
    for (auto e : edges) {
        add_edge(e.first, e.second, g);
    }
    return g;
}
template <typename T>
void outputGraph(ostream& fout, const T& g) {
    //Print graph to stdout, unlabelled
    write_graphviz(fout, g);

}

BaseGraph readGraph(istream& fin) {
    BaseGraph g(0);
    dynamic_properties dp(ignore_other_properties);
    read_graphviz(fin, g, dp);
    return g;

}