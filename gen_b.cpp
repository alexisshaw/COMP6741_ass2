#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <random>

#include <boost/range/iterator_range.hpp>
#include <boost/graph/random>

#include "gen_types.h"
#include "kernelisation/graph_serialization.h"

using namespace std;
using namespace boost;

int main(int argc, char* argv[]){
    int n = 1000;
    int k = 10;

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
    shuffle(begin(vertex_descriptors), end(vertex_descriptors), mt);

    Graph g(n);
    adjGraph g_adj(n);
    Graph available(n);

    //setup output graph
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

    for (int i=0; i < k; i++){
        for (int v : iterator_range(begin(vertex_descriptors) + x[i]+1, begin(vertex_descriptors) + x[i] + len[i])){
            g_adj[edge(v,vertex_descriptors[x[i]],g_adj).first].minDist = 1;
        }
        for (int v1 : iterator_range(begin(vertex_descriptors) + x[i]+1, begin(vertex_descriptors) + x[i] + len[i])){
            for (int v2 : iterator_range(begin(vertex_descriptors) + x[i]+1, begin(vertex_descriptors) + x[i] + len[i])){
                if(v1 != v2) {
                    g_adj[edge(v1, v2, g_adj).first].minDist = 1;
                }
            }
        }
    }

    //setup available edges graph
    for (int i=0; i < k; ++i){
        for (int v1 : iterator_range(begin(vertex_descriptors) + x[i], begin(vertex_descriptors) + x[i] + len[i])){
            for(int j = (i+1);j < k; ++j){
                for (int v2 : iterator_range(begin(vertex_descriptors) + x[j] + 1, begin(vertex_descriptors) + x[j] + len[j])){
                    add_edge(v1, v2, available);
                }
            }
        }
    }

    while(num_edges(available) > 0){
        auto e = random_edge(available, Gen());
        int v1 = source(e, available);
        int v2 = target(e, available);

        for (int vn : adjacent_vertices(v1, available)){
            if(vn != v2) {
                int d_1 = g_adj[edge(v1, vn, g_adj).first].minDist;
                int d_2 = g_adj[edge(v2, vn, g_adj).first].minDist;
                int loop = g_adj[edge(v1, vn, g_adj).first].minLoopLen;

                g_adj[edge(v1, vn, g_adj).first].minDist = min(d_1, d_2 + 1);
                g_adj[edge(v1, vn, g_adj).first].minLoopLen = min(loop, d_1 + d_2 + 1);//this needs to be fixed, incorrect. make work.

                if (((d2 + 1) < 4) || ((d1 + d2 + 1) < 10)) {
                    remove_edge(v1, vn, available);
                }
            }
        }

        for (int vn : adjacent_vertices(v2, available)){
            if (vn != v1) {
                int d_1 = g_adj[edge(v2, vn, g_adj).first].minDist;
                int d_2 = g_adj[edge(v1, vn, g_adj).first].minDist;
                int loop = g_adj[edge(v1, vn, g_adj).first].minLoopLen;

                g_adj[edge(v2, vn, g_adj).first].minDist = min(d_1, d_2 + 1);
                g_adj[edge(v2, vn, g_adj).first].minLoopLen = min(loop, d_1 + d_2 + 1);//this needs to be fixed, incorrect. make work.

                if (((d2 + 1) < 4) || ((d1 + d2 + 1) < 10)) {
                    remove_edge(v2, vn, available);
                }
            }
        }
        add_edge(v1, v2, g);
        remove_edge(v1, v2, available);
    }
    


    return 0;
}