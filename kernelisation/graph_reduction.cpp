#include "types.h"
#include "graph_reduction.h"
#include <assert.h>

using namespace std;
using namespace boost;

namespace {
	unsigned count_red_verticies(Graph& g) {
		unsigned n = 0;
		Graph::vertex_iterator vi, vi_end;
		for (boost::tie(vi, vi_end) = vertices(g); vi != vi_end; ++vi) {
			if (g[*vi].color == VertexColor::RED)
				n++;
		}
		return n;
	}

	void delete_edge_between(Graph& g, Graph::vertex_descriptor v1, Graph::vertex_descriptor v2, stack<GraphModifier>& modifier_stack) {
		pair<Graph::edge_descriptor, bool> ep = edge(v1, v2, g);
		assert(ep.second == true);
		modifier_stack.push(GraphModifier(GraphModifierType::ADD_EDGE, ep.first));
		g[ep.first].hidden = true;
	}

	void delete_vertex(Graph& g, Graph::vertex_descriptor v, stack<GraphModifier>& modifier_stack) {
		modifier_stack.push(GraphModifier(GraphModifierType::ADD_VERTEX, v));
		g[v].hidden = true;
	}

	void color_vertex(Graph& g, Graph::vertex_descriptor v, VertexColor c, stack<GraphModifier>& modifier_stack) {
		GraphModifierType t;

		//Set type based on the original color of the vertex
		switch (g[v].color) {
		case VertexColor::BLACK:
			t = GraphModifierType::COLOR_VERTEX_BLACK;
			break;
		case VertexColor::WHITE:
			t = GraphModifierType::COLOR_VERTEX_WHITE;
			break;
		case VertexColor::RED:
			t = GraphModifierType::COLOR_VERTEX_RED;
			break;
		}

		modifier_stack.push(GraphModifier(t, v));
		g[v].color = c;
	}

	unsigned color_adjacent_verticies(Graph& g, Graph::vertex_descriptor v, VertexColor c, stack<GraphModifier>& modifier_stack) {
		unsigned modifications = 0;
		//Graph::vertex_iterator v2i, v2i_end;
		//for (boost::tie(v2i, v2i_end) = adjacent_vertices(v, g); v2i != v2i_end; ++v2i) {
		for (pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vp = adjacent_vertices(v, g); vp.first != vp.second; ++vp.first) {
			color_vertex(g, *vp.first, VertexColor::WHITE, modifier_stack);
			modifications++;
		}
		return modifications;
	}

	void kernelize_vertex(Graph &g, Graph::vertex_descriptor &v1, stack<GraphModifier>& modifier_stack, unsigned k, unsigned red_size) {
		VertexColor v1c = g[v1].color;
		if (v1c == VertexColor::RED)
			return;

		//Iterate through the neighbours of v1 and apply the three reduction rules
		unsigned black_neighbours = 0;
		for (pair<Graph::adjacency_iterator, Graph::adjacency_iterator> vp2 = adjacent_vertices(v1, g); vp2.first != vp2.second; ++vp2.first) {
			Graph::vertex_descriptor v2 = *vp2.first;
			VertexColor v2c = g[v2].color;

			if (v2c == VertexColor::BLACK)
				black_neighbours++;

			//Reduction Rule R3
			if (v1c == VertexColor::WHITE && v2c == VertexColor::WHITE)
				delete_edge_between(g, v1, v2, modifier_stack);
		}

		//Reduction Rule R2
		if (v1c == VertexColor::WHITE && black_neighbours == 0)
			delete_vertex(g, v1, modifier_stack);

		//Reduction Rule R1
		if (black_neighbours > (k - red_size)) {
			color_vertex(g, v1, VertexColor::RED, modifier_stack);
			color_adjacent_verticies(g, v1, VertexColor::WHITE, modifier_stack);
		}
	}


}

namespace GraphReduction {
	pair<bool, unsigned> kernelize(Graph& g, unsigned k, stack<GraphModifier>& modifier_stack) {
		unsigned stack_size = modifier_stack.size();
		unsigned red_size = count_red_verticies(g);

		//Reduction Rule R4
		if (red_size > k)
			return make_pair(false, 0);
		
		//Iterate though all verticies v1 and kerenlize them
		for (pair<Graph::vertex_iterator, Graph::vertex_iterator> vp1 = vertices(g); vp1.first != vp1.second; ++vp1.first) {
			Graph::vertex_descriptor v1 = *vp1.first;
			kernelize_vertex(g, v1, modifier_stack, k, red_size);
		}

		return make_pair(true, modifier_stack.size() - stack_size);
	}

	void unkernelize(Graph& g, stack<GraphModifier>& modifier_stack, unsigned m) {
		//Pop off the top m modifiers and apply their action
		for (unsigned i = 0; i < m; i++) {
			GraphModifier m = modifier_stack.top();
			switch (m.type) {
			case GraphModifierType::COLOR_VERTEX_BLACK:
				g[m.v].color = VertexColor::BLACK;
				break;
			case GraphModifierType::COLOR_VERTEX_WHITE:
				g[m.v].color = VertexColor::WHITE;
				break;
			case GraphModifierType::COLOR_VERTEX_RED:
				g[m.v].color = VertexColor::RED;
				break;
			case GraphModifierType::ADD_VERTEX:
				g[m.v].hidden = false;
				break;
			case GraphModifierType::ADD_EDGE:
				g[m.e].hidden = false;
				break;
			}
			modifier_stack.pop();
		}
	}
}