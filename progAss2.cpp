#include <boost/graph>
#include <boost/range>
#include <bits/stl_bvector.h>
#include "kernelisation/types.h"
#include "kernelisation/graph_reduction.h"
#include "combinations/combination.h"

int main(int argc, char* argv[]){

}

bool brute(Graph g, int k){
    vector<Graph::vertex_descriptor> black_white_descriptors;
    int numUndominated = 0;

    for (auto v : boost::make_iterator_range(vertices(g))){
        numUndominated ++;
        if(g[v].color == VertexColor::RED){
            for (auto v1 : boost::make_iterator_range(adjacent_vertices(v, g))){
                if(g[v1].redNeighbourCount == 0)
                    numUndominated--;
                g[v1].redNeighbourCount++;
            }
        } else {
            black_white_descriptors.push_back(v);
        }
    }


    for(auto comb: combination<Graph::vertex_descriptor>(black_white_descriptors)){
        if (comb.lastIn() == comb.lastOut){
            for (auto v : comb){
                for (auto v1 : boost::make_iterator_range(adjacent_vertices(v, g))){
                    if(g[v1].redNeighbourCount == 0){
                        numUndominated--;
                    }
                    g[v1].redNeighbourCount++;
                }
            }
        } else {
            auto out = comb.lastOut();
            for (auto v1 : boost::make_iterator_range(adjacent_vertices(out, g))){
                g[v1].redNeighbourCount--;
                if(g[v1].redNeighbourCount == 0) {
                    numUndominated++;
                }
            }

            auto in = comb.lastOut();
            for (auto v1 : boost::make_iterator_range(adjacent_vertices(in, g))){
                if(g[v1].redNeighbourCount == 0){
                    numUndominated--;
                }
                g[v1].redNeighbourCount++;
            }
        }

        if(numUndominated == 0){
            return true;
        }
    }

    return false;
}