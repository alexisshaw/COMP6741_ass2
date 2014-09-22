#include <iostream>
#include "types.h"
#include <boost\graph\graph_selectors.hpp>
#include <boost\graph\graph_utility.hpp>

using namespace std;

void print_graph(Graph& g, const char* name) {
	cout << "Vertex set: ";
	print_vertices(g, name);
	cout << "Edge set: ";
	print_edges(g, name);
	cout << endl;
}

int main()
{
	//Build base graph
	enum { A, B, C, D, E, N };
	const char* name = "ABCDE";
	BaseGraph base_graph(N);
	add_edge(A, B, base_graph);
	add_edge(A, C, base_graph);
	add_edge(C, D, base_graph);
	add_edge(C, E, base_graph);
	add_edge(D, B, base_graph);
	add_edge(E, C, base_graph);

	//Create filtered view of graph which will be used exclusively in algorithms
	cout << "Creating filtered graph... " << endl;
	Graph g(base_graph);
	print_graph(g, name);

	//Perform a few basic tests of filtering
	cout << "Hiding vertex A..." << endl;
	Graph::vertex_descriptor first = *vertices(g).first;
	g[first].color = VertexColor::RED;
	g[first].hidden = true;	

	print_graph(g, name);
	
	cout << "Restoring vertex A..." << endl;
	g[first].hidden = false;

	print_graph(g, name);
	
	cout << "Hiding edges from A..." << endl;
	vector<Graph::edge_descriptor> edges_to_restore;
	for (pair<Graph::out_edge_iterator, Graph::out_edge_iterator> ep = out_edges(first, g); ep.first != ep.second; ++ep.first) {
		Graph::edge_descriptor e = *ep.first;
		g[e].hidden = true;
		edges_to_restore.push_back(e);
	}

	print_graph(g, name);

	cout << "Restoring edges from A..." << endl;
	for (auto e : edges_to_restore) {
		g[e].hidden = false;
	}

	print_graph(g, name);

	cout << sizeof(GraphModifierType);

	return 0;
}