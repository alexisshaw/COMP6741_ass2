#pragma once

#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>

#include <random>

using namespace boost;

struct EdgeProperty
{
    char minDist = 255;
    char minLoopLen = 255;
};

struct Gen {
    random_device rd;
    Gen()
            : g(static_cast<uint32_t>(time(0)))
    {
    }
    size_t operator()(size_t n)
    {
        std::uniform_int_distribution<size_t> d(0, n ? n-1 : 0);
        return d(g);
    }
};

//Define the final graph type
typedef adjacency_matrix<undirectedS, no_property, EdgeProperty> dataGraph;
typedef adjacency_list<vecS, vecS, undirectedS> Graph;