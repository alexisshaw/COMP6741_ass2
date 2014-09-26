#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <random>

#include <boost/range/iterator_range.hpp>
#include <boost/graph/random.hpp>

#include "gen_types.h"
#include "kernelisation/graph_serialization.h"

using namespace std;
using namespace boost;

int main(int argc, char* argv[]){
    int n = 1000;
    int k = 10;
    int minG = 5;

    //get partitions
    vector<int> vertex_descriptors(n-k);
    iota(begin(vertex_descriptors), end(vertex_descriptors), 0);

    random_device rd;

    shuffle(begin(vertex_descriptors), end(vertex_descriptors), rd);

    vector<int> x(k);
    vector<int> y(k);
    vector<int> len(k);

    x[0] = 0;
    y[0] = vertex_descriptors[n-k];
    for(int i=1; i<k; ++i){
        x[i] = vertex_descriptors[i-1] + i;
        y[i] = vertex_descriptors[n-k+i];
        len[i-1] = x[i] - x[i-1];
    }
    len[k-1] = n - x[k-2];

    vertex_descriptors.resize(n);
    iota(begin(vertex_descriptors)+(n-k), end(vertex_descriptors), n-k);
    shuffle(begin(vertex_descriptors), end(vertex_descriptors), rd);

    Graph g(n);
    adjGraph g_adj(n);
    Graph available(n);

    //setup output graph, that is connect the x[i] to every other vertex in the partition.
    for (int i = 0; i < k; ++i){
        for (int v : iterator_range(begin(vertex_descriptors) + x[i]+1, begin(vertex_descriptors) + x[i] + len[i])){
            add_edge(vertex_descriptors[x[i]], v, g);
        }
        add_edge(vertex_descriptors[x[i]],vertex_descriptors[n-k+i], g);
    }

    //Setup distance lookup graph
    for (int i = 0 ; i < n; ++i){
        for (int j = (i+1); i < n; ++j){
            add_edge(i,j,g_adj);
        }
    }


    //Setup initial shortest Distances
    for (int i=0; i < k; i++){
        //The shortest Distance to an x[i] within it's partition is 1
        for (int v : iterator_range(begin(vertex_descriptors) + x[i]+1, begin(vertex_descriptors) + x[i] + len[i])){
            pair<adjGraph::edge_descriptor, bool> e = edge(v, vertex_descriptors[x[i]], g_adj);
            if(e.second) {
                g_adj[e.first].minDist = 1;
            } else {
                abort();
            }
        }
        //The shortest distance within a partition to a non x[i] is 2 via the x[i]
        for (int v1 : iterator_range(begin(vertex_descriptors) + x[i]+1, begin(vertex_descriptors) + x[i] + len[i])){
            for (int v2 : iterator_range(begin(vertex_descriptors) + x[i]+1, begin(vertex_descriptors) + x[i] + len[i])){
                if(v1 != v2) {
                    pair<adjGraph::edge_descriptor, bool> e = edge(v1, v2, g_adj);
                    if(e.second) {
                        g_adj[e.first].minDist = 2;
                    } else {
                        abort();
                    }
                }
            }
        }
    }


    //setup available edges graph, Initially you can connect to anyone outside your partition.
    for (int i=0; i < k; ++i){
        for (int v1 : iterator_range(begin(vertex_descriptors) + x[i], begin(vertex_descriptors) + x[i] + len[i])){
            for(int j = (i+1);j < k; ++j){
                for (int v2 : iterator_range(begin(vertex_descriptors) + x[j] + 1, begin(vertex_descriptors) + x[j] + len[j])){
                    add_edge(v1, v2, available);
                }
            }
        }
    }

    vector<adjGraph::edge_descriptor> toMark;

    while(num_edges(available) > 0){
        Gen g = Gen();
        auto e = random_edge(available, g);
        int v1 = source(e, available);
        int v2 = target(e, available);

        remove_edge(v1, v2, available);

        for (int vn : iterator_range(adjacent_vertices(v1, available))){
            pair<adjGraph::edge_descriptor, bool> e1 = edge(vn, v1, g_adj);
            pair<adjGraph::edge_descriptor, bool> e2 = edge(vn, v2, g_adj);

            if(e1.second && e2.second) {
                int minDist1 = g_adj[e1.first].minDist;
                int minDist2 = g_adj[e2.first].minDist;

                minDist1 = (g_adj[e1.first].minDist = min(minDist1, 1+minDist2));

                if (minDist1 < minG){
                    remove_edge(v1, vn, available);
                }
            } else {
                abort();
            }
        }

        for (int vn : iterator_range(adjacent_vertices(v2, available))){
            pair<adjGraph::edge_descriptor, bool> e1 = edge(vn, v2, g_adj);
            pair<adjGraph::edge_descriptor, bool> e2 = edge(vn, v1, g_adj);

            if(e1.second && e2.second) {
                int minDist1 = g_adj[e1.first].minDist;
                int minDist2 = g_adj[e2.first].minDist;

                minDist1 = (g_adj[e1.first].minDist = min(minDist1, 1 + minDist2));

                if (minDist1 < minG){
                    remove_edge(v2, vn, available);
                }
            } else {
                abort();
            }
        }
        add_edge(v1, v2, g);

    }


    return 0;
}