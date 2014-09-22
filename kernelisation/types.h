#pragma once

#include <boost\graph\filtered_graph.hpp>
#include <boost\graph\adjacency_list.hpp>

using namespace boost;

//Define Vertex and Edge properties. Both have a hidden property which is used in the final, filtered, graph
enum class VertexColor { BLACK, WHITE, RED };
struct VertexProperty
{
	bool hidden = false;
	VertexColor color = VertexColor::BLACK;
};
struct EdgeProperty
{
	bool hidden = false;
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
		filtered_graph<Graph, HiddenFilter<Graph>, HiddenFilter<Graph>>(g, HiddenFilter<Graph>(g), HiddenFilter<Graph>(g)) {}
};

//Define the final graph type
typedef adjacency_list<vecS, vecS, undirectedS, VertexProperty, EdgeProperty> BaseGraph;
typedef HiddenFilteredGraph<BaseGraph> Graph;


//Graph modifiers are used to undo the actions of the kernelization algorithm
enum class GraphModifierType {
	COLOR_VERTEX_BLACK,
	COLOR_VERTEX_WHITE,
	COLOR_VERTEX_RED,
	ADD_VERTEX,
	ADD_EDGE
};

struct GraphModifier {
	GraphModifier(GraphModifierType type_in, Graph::vertex_descriptor v_in) : type(type_in), v(v_in) {}
	GraphModifier(GraphModifierType type_in, Graph::edge_descriptor e_in) : type(type_in), e(e_in) {}
	
	GraphModifierType type;
	//Cannot union a edge descriptor, it has a user-defined constructor.
	//union {
		Graph::vertex_descriptor v;
		Graph::edge_descriptor e;
	//};
};