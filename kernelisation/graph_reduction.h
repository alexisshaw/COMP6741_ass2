#pragma once

#include "types.h"
#include <stack>

using namespace std;

namespace {
    using namespace std;
    using namespace boost;
    using namespace reductionTypes;

    template <typename T>
    inline void delete_edge_between(Graph& g, Graph::vertex_descriptor v1, Graph::vertex_descriptor v2, T& modifier_stack) {
        auto ep = edge(v1, v2, g);
        assert(ep.second == true);
        modifier_stack.push(GraphModifier(GraphModifierType::ADD_EDGE, ep.first));
        g[ep.first].hidden = true;
    }

    template <typename T>
    inline void delete_vertex(Graph& g, Graph::vertex_descriptor v, T& modifier_stack) {
        modifier_stack.push(GraphModifier(GraphModifierType::ADD_VERTEX, v));
        g[v].hidden = true;
    }

    template<typename T>
    inline void kernelize_vertex(Graph &g, Graph::vertex_descriptor v1, T& modifier_stack, int k) {
        VertexProperty v1p = g[v1];
        if (v1p.isRed)
            return;

        //Iterate through the neighbours of v1 and apply the three reduction rules
        int black_neighbours = 0;
        for (auto v2 : boost::make_iterator_range(adjacent_vertices(v1, g))) {
            VertexProperty v2p = g[v2];
            if (v2p.redNeighbourCount == 0)
                black_neighbours++;

            //Reduction Rule R3
            if ((v1p.redNeighbourCount != 0) && (v2p.redNeighbourCount != 0 && !v2p.isRed))
                delete_edge_between(g, v1, v2, modifier_stack);
        }

        //Reduction Rule R2
        if ((v1p.redNeighbourCount != 0) && (black_neighbours == 0))
            delete_vertex(g, v1, modifier_stack);

        //Reduction Rule R1
        if (black_neighbours > (k - g[graph_bundle].numRed)) {
            //color_vertex(g, v1, VertexColor::RED, modifier_stack);
            //color_adjacent_verticies(g, v1, VertexColor::WHITE, modifier_stack);
            makeRed(v1, g);
            modifier_stack.push(GraphModifier(GraphModifierType::COLOR_RED, v1));
        }
    }
}

namespace GraphReduction {
    using namespace std;
    using namespace boost;
    using namespace reductionTypes;

    template<typename T>
    inline void kernelize(Graph& g, int k, T& modifier_stack) {
        //Iterate though all verticies v1 and kerenlize them
        for (auto v : boost::make_iterator_range(vertices(g))) {
            kernelize_vertex(g, v, modifier_stack, k);
        }
    }

    template<typename T>
    inline void unkernelize(Graph& g, T& modifier_stack) {
        //Pop off the top m modifiers and apply their action
        while (!modifier_stack.empty()) {
            GraphModifier m = modifier_stack.top();
            modifier_stack.pop();

            switch (m.type) {
                case GraphModifierType::COLOR_RED:
                    makeRed(m.v, g);
                    break;
                case GraphModifierType::UN_COLOR_RED:
                    makeUnRed(m.v, g);
                    break;
                case GraphModifierType::ADD_VERTEX:
                    g[m.v].hidden = false;
                    break;
                case GraphModifierType::ADD_EDGE:
                    g[m.e].hidden = false;
                    break;
            }
        }
    }
}
