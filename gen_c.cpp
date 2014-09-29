#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <random>
#include <tuple>

#include <boost/range/iterator_range.hpp>
#include <boost/graph/random.hpp>
#include <boost/lexical_cast.hpp>

#include "gen_types.h"
#include "gen_b.h"
#include "kernelisation/graph_serialization.h"
#include "availableList.h"

using namespace std;
using namespace boost;

using namespace genTypes;

int main(int argc, char* argv[]) {
    const int n = 1000;
    const int k = 10;
    const int minG = 5;

    vector<Graph::vertex_descriptor> X = getX(n, k);
    vector<Graph::vertex_descriptor> Y = getY(X);
    vector<vector<genTypes::Graph::vertex_descriptor>> V = getV(X, n);

    Graph g(n);

    //setup output graph, that is connect the x[i] to every other vertex in the partition.
    for (int i = 0; i < k; ++i) {
        for (int v : V[i]) {
            add_edge(X[i], v, g);
        }
        add_edge(X[i], Y[i], g);
    }


    {
        ofstream outputFile(boost::lexical_cast<std::string>(std::time(nullptr)) + "s.dot");
        outputGraph(outputFile, g);
    }


    avilableList a(X,V);
    a.updateAvailable(g);

    random_device rd;
    while(num_edges(available) > 0){
        uniform_int_distribution<int> dist(0, a.size() - 1);
        Graph::vertex_descriptor v1, v2;
        tie(v1, v2) = a[dist(rd)];

        cout << v1 << ' ' << v2 << ' ';

        add_edge(v1, v2, g);

        a.updateAvailable(v1, v2, g);

        cout << " Edges: " << num_edges(g) << " Available Remaining: " << num_edges(available) << endl;
    }


    {
        ofstream outputFile(boost::lexical_cast<std::string>(std::time(nullptr)) + "e.dot");
        outputGraph(outputFile, g);
    }

    return 0;
}