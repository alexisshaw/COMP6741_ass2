#include <boost/graph>
#include <boost/range>
#include <bits/stl_bvector.h>
#include "kernelisation/types.h"
#include "kernelisation/graph_reduction.h"
#include "combinations/combination.h"

bool brute(Graph g, int k){
    vector<Graph::vertex_descriptor> non_red_descriptors;

    for (auto v : boost::make_iterator_range(vertices(g))){
        if(!g[v].isRed){
            non_red_descriptors.push_back(v);
        }
    }


    for(auto comb: combination<Graph::vertex_descriptor>(non_red_descriptors)){
        if (comb.lastIn() == comb.lastOut){
            for (auto v : comb){
                makeRed(v, g, numUndominated);
            }
        } else {
            auto out = comb.lastOut();
            makeUnRed(in,g,numUndominated);

            auto in = comb.lastOut();
            makeRed(in, g, numUndominated);
        }

        if(g[graph_bundle].numBlack == 0){
            return true;
        }
    }

    return false;
}