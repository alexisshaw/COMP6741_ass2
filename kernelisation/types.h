#pragma once

#include <boost/graph/filtered_graph.hpp>
#include <boost/graph/adjacency_list.hpp>

using namespace boost;

//Define Vertex and Edge properties. Both have a hidden property which is used in the final, filtered, graph
enum class VertexColor { BLACK, WHITE, RED };
struct VertexProperty
{
    bool hidden = false;
    bool isRed = false;
    int redNeighbourCount = 0;
};
struct EdgeProperty
{
    bool hidden = false;
};

struct GraphProperty{
    int numRed = 0;
    int numBlack = 0;
};

//Define filter which is used on both verticies and edges. Filters hidden verticies/edges.
template <typename Graph>
class HiddenFilter {
public:
    HiddenFilter() { /*throw "This shouldn't happen!";*/ }
    HiddenFilter(Graph &g) : m_graph(&g) { }
    HiddenFilter& operator= (const HiddenFilter other) {
        m_graph = other.m_graph;
        return *this;
    }

    template <typename T>
    bool operator()(const T& e) const {
        return !(*m_graph)[e].hidden;
    }
private:
    Graph* m_graph;
};

//Define a class which takes a graph and filters it with the HiddenFilter
template <typename Graph>
struct HiddenFilteredGraph : public filtered_graph<Graph, HiddenFilter<Graph>, HiddenFilter<Graph>> {
    HiddenFilteredGraph(Graph& g) :
            filtered_graph<Graph, HiddenFilter<Graph>, HiddenFilter<Graph>>(g, HiddenFilter<Graph>(g), HiddenFilter<Graph>(g)) {
        g[graph_bundle].numBlack = num_vertices(g); //Assuming input graph has every vertex black
    }
};

//Define the final graph type
typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty, GraphProperty> BaseGraph;
typedef HiddenFilteredGraph<BaseGraph> Graph;


//Graph modifiers are used to undo the actions of the kernelization algorithm
enum class GraphModifierType {
    COLOR_RED,
    UN_COLOR_RED,
    ADD_VERTEX,
    ADD_EDGE
};

struct GraphModifier {
    GraphModifier(GraphModifierType type_in, Graph::vertex_descriptor v_in) : type(type_in), v(v_in) {}
    GraphModifier(GraphModifierType type_in, Graph::edge_descriptor e_in) : type(type_in), e(e_in) {}
    GraphModifier(){};

    GraphModifierType type;
    //Cannot union a edge descriptor, it has a user-defined constructor.
    //union {
    Graph::vertex_descriptor v;
    Graph::edge_descriptor e;
    //};
};

inline void makeRed(Graph::vertex_descriptor v, Graph& g) {
    if (g[v].redNeighbourCount == 0){
        g[graph_bundle].numBlack--;
    }
    g[v].redNeighbourCount++;

    g[v].isRed = true;
    g[graph_bundle].numRed++;

    for (auto v1 : boost::make_iterator_range(adjacent_vertices(v, g))){
        if (g[v1].redNeighbourCount == 0){
            g[graph_bundle].numBlack--;
        }
        g[v1].redNeighbourCount++;
    }
}

inline void makeUnRed(Graph::vertex_descriptor v, Graph& g) {
    g[v].redNeighbourCount--;

    g[v].isRed = false;
    g[graph_bundle].numRed--;

    for (auto v1 : boost::make_iterator_range(adjacent_vertices(v, g))){
        g[v1].redNeighbourCount--;
        if (g[v1].redNeighbourCount == 0){
            g[graph_bundle].numBlack++;
        }
    }
}