#include <boost/graph>
#include <boost/range>
#include <bits/stl_bvector.h>
#include <stack>
#include "kernelisation/types.h"
#include "kernelisation/graph_reduction.h"
#include "combinations/combination.h"



bool branch(Graph g, int k, bool doKern, vector<stack<GraphModifierType, vector<GraphModifierType>>>& stacks){
    vector<Graph::vertex_descriptor> black_descriptors;

    if (g[graph_bundle].numBlack == 0 ) return k >=  g[graph_bundle].numRed;
    if (k <= g[graph_bundle].numRed) return false;

    for (auto v : boost::make_iterator_range(vertices(g))){
        if(!g[v].isRed){
            non_red_descriptors.push_back(v);
        }
    }

    stack<GraphModifierType, vector<GraphModifierType>> kerneliseState;
    std::swap(kerneliseState, stacks.back());
    stacks.pop;
    stacks.pop_back();

    if (doKern) {
        int oldSize = kerneliseState.size();
        kernelize(g, k, kerneliseState);

        if (g[graph_bundle].numBlack == 0 ) return k >=  g[graph_bundle].numRed;
        if (k <= g[graph_bundle].numRed) return false;
        while(oldSize < kerneliseState.size()){
            kernelize(g, k, kerneliseState);

            if (g[graph_bundle].numBlack == 0 ) return k >=  g[graph_bundle].numRed;
            if (k <= g[graph_bundle].numRed) return false;
        }
    }

    Graph::vertex_descriptor v;

    if (g[graph_bundle].numBlack == 0 ) return k >=  g[graph_bundle].numRed;
    if (k <= g[graph_bundle].numRed) return false;

    for (auto v1 : boost::make_iterator_range(vertices(g))){
        if(g[v1].redNeighbourCount == 0){
            v = v1;
            break;
        }
    }

    makeRed(v,g);
    if(branch(g,k,doKern, stacks)) return true;
    makeUnRed(v,g);

    for (auto w : boost::make_iterator_range(adjacent_vertices(v, g))){
        makeRed(w,g);
        if(branch(g,k,doKern, stacks)) return true;
        makeUnRed(w,g);
    }

    if(doKern)
        unkernelize(g,k, kerneliseState);
    stacks.push_back(move(kerneliseState));

    return false;
}