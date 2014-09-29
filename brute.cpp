#include <boost/range/iterator_range.hpp>
#include <bits/stl_bvector.h>
#include "kernelisation/types.h"
#include "kernelisation/graph_reduction.h"
#include "combinations/combination.h"
#include <iostream>
#include <vector>

using namespace std;

bool brute(Graph g, int k){
    vector<Graph::vertex_descriptor> non_red_descriptors;

    for (Graph::vertex_descriptor v : boost::make_iterator_range(vertices(g))){
        if(!g[v].isRed){
            non_red_descriptors.push_back(v);
        }
    }

    if (g[graph_bundle].numBlack == 0 ) return k >=  g[graph_bundle].numRed;
    if (k <= g[graph_bundle].numRed) return false;


    for(auto comb: combination<Graph::vertex_descriptor>(non_red_descriptors,k - g[graph_bundle].numRed)){
        if (comb.lastIn() == comb.lastOut()){
            for (auto v : comb){
                makeRed(v, g);
            }
        } else {
            auto out = comb.lastOut();
            makeUnRed(out,g);

            auto in = comb.lastIn();
            makeRed(in, g);
        }

        if(g[graph_bundle].numBlack == 0){
            for (auto v : comb){
                std::cout << v << " ";
            }
            std::cout << endl;
            return true;
        }
    }

    return false;
}