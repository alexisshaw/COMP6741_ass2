#pragma once

#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>


#include <vector>
#include <random>
#include <climits>

using namespace boost;
using namespace std;

struct EdgeProperty
{
    int minDist = (INT_MAX - 2);

    bool mark;
};

struct Gen {
    random_device rd;
    size_t operator()(size_t n)
    {
        std::uniform_int_distribution<size_t> d(0, n ? n-1 : 0);
        return d(rd);
    }
};

//Define the final graph type
typedef adjacency_matrix<undirectedS, no_property, EdgeProperty> adjGraph;
typedef adjacency_list<vecS, vecS, undirectedS> Graph;