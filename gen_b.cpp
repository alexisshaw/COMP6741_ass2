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
    const int n = 1000;
    const int k = 10;
    const int minG = 5;

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
        y[i] = n-k+i;
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
        for (int v : iterator_range(begin(vertex_descriptors) + x[i] + 1, begin(vertex_descriptors) + x[i] + len[i])){
            add_edge(vertex_descriptors[x[i]], v, g);
        }
        add_edge(vertex_descriptors[x[i]],vertex_descriptors[y[i]], g);
    }

    //Setup distance lookup graph (complete graph!)
    for (int i = 0 ; i < n; ++i){
        for (int j = (i+1); j < n; ++j){
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
                for (int v2 : iterator_range(begin(vertex_descriptors) + x[j], begin(vertex_descriptors) + x[j] + len[j])){
                    add_edge(v1, v2, available);
                }
            }
        }
    }

    auto randGen = Gen();

    while(num_edges(available) > 0){
        auto e = random_edge(available, randGen);
        int v1 = source(e, available);
        int v2 = target(e, available);

        add_edge(v1, v2, g);

        //update distances in new graph
        for (auto e : iterator_range(edges(g_adj))){
            auto vn1 = target(e, g_adj);
            auto vn2 = source(e, g_adj);

            int dOld = g_adj[e];

            if ((vn1 != v1) && (vn2 != v1) && (vn1 != v2) && (vn2 != v2)) {
                auto evn1v1 = edge(vn1, v1, g_adj);
                auto evn1v2 = edge(vn1, v2, g_adj);
                auto evn2v1 = edge(vn2, v1, g_adj);
                auto evn2v2 = edge(vn2, v2, g_adj);

                assert(evn1v1.second && evn1v2.second && evn2v1.second && evn2v2.second);

                int dvn1v1 = g_adj[evn1v1.first].minDist;
                int dvn1v2 = g_adj[evn1v2.first].minDist;
                int dvn2v1 = g_adj{evn2v1.first}.minDist;
                int dvn2v2 = g_adj[evn2v1.first].minDist;
                g_adj[e] = min(dOld, min(dvn1v1 + dvn2v2 + 1, dvn2v1 + dvn1v2 + 1));
            } else if ((vn1 == v1) && (vn2 != v2) ){
                auto evn2v2 = edge(vn2,v2,g_adj);

                assert(evn2v2.second);

                g_adj[e].minDist == min(dOld, 1 + g_adj[evn2v2].minDist);
            } else if ((vn1 == v2) && (vn2 != v1)) {
                auto evn2v1 = edge(vn2,v1,g_adj);

                assert(evn2v1.second);

                g_adj[e].minDist == min(dOld, 1 + g_adj[evn2v1].minDist);
            } else if ((vn2 == v1) && (vn1 != v2)) {
                auto evn1v2 = edge(vn1,v2,g_adj);

                assert(evn1v2.second);

                g_adj[e].minDist == min(dOld, 1 + g_adj[evn1v2].minDist);
            } else if ((vn2 == v2) && (vn1 != v1)) {
                auto evn1v1 = edge(vn1,v1,g_adj);

                assert(evn1v1.second);

                g_adj[e].minDist == min(dOld, 1 + g_adj[evn1v1].minDist);
            } else if (((vn1 == v1) && (vn2 == v2))||((vn1 == v2) && (vn2 == v1))){
                g_adg[e].minDist == 1;
            }
        }

        //remove edges that would make the graph girth minG - 1 from available.
        for (auto vn : iterator_range(adjacent_vertices(v1, available))){
            auto e = edge(vn, v1, g_adj);
            assert(e.second);
            if (g_adj[e].minDist < (minG - 1)){
                remove_edge (v1, vn, available);
            }
        }

        for (auto vn : iterator_range(adjacent_vertices(v2, available))){
            auto e = edge(vn, v2, g_adj);
            assert(e.second);
            if (g_adj[e].minDist < (minG - 1)){
                remove_edge (v2, vn, available);
            }
        }
    }

    output_graph(cout, g);


    return 0;
}