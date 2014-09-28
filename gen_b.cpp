#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <random>

#include <boost/range/iterator_range.hpp>
#include <boost/graph/random.hpp>
#include <boost/lexical_cast.hpp>

#include "gen_types.h"
#include "gen_b.h"
#include "kernelisation/graph_serialization.h"

using namespace std;
using namespace boost;

using namespace genTypes;

int main(int argc, char* argv[]) {
    const int n = 1000;
    const int k = 10;
    const int minG = 5;

    vector<unsigned long> X = getX(n, k);
    vector<unsigned long> Y = getY(X);
    vector<vector<unsigned long>> V = getV(X, n);

    Graph g(n);
    adjGraph g_adj(n);
    Graph available(n);

    //setup output graph, that is connect the x[i] to every other vertex in the partition.
    for (int i = 0; i < k; ++i) {
        for (int v : V[i]) {
            add_edge(X[i], v, g);
        }
        add_edge(X[i], Y[i], g);
    }

    //Setup distance lookup graph (complete graph!)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < i; ++j) {
            add_edge(i, j, g_adj);

            pair<adjGraph::edge_descriptor, bool> e = edge(i, j, g_adj);
            if (e.second) {
                g_adj[e.first].minDist = (INT_MAX - 2) / 2;
            } else {
                abort();
            }
        }
    }


    //Setup initial shortest Distances
    for (int i = 0; i < k; i++) {
        //The shortest Distance to an x[i] within it's partition is 1
        for (int v : V[i]) {
            pair<adjGraph::edge_descriptor, bool> e = edge(v, X[i], g_adj);
            if (e.second) {
                g_adj[e.first].minDist = 1;
            } else {
                abort();
            }
        }
        //distance between Y[i] and X[i]
        {
            pair<adjGraph::edge_descriptor, bool> e = edge(Y[i], X[i], g_adj);
            if (e.second) {
                g_adj[e.first].minDist = 1;
            } else {
                abort();
            }
        }

        //The shortest Distance to an Y[i] within it's partition is 2
        for (int v : V[i]) {
            pair<adjGraph::edge_descriptor, bool> e = edge(v, Y[i], g_adj);
            if (e.second) {
                g_adj[e.first].minDist = 2;
            } else {
                abort();
            }
        }

        //The shortest distance within a partition to a non x[i] is 2 via the x[i]
        for (int v1 : V[i]) {
            for (int v2 : V[i]) {
                if (v1 != v2) {
                    pair<adjGraph::edge_descriptor, bool> e = edge(v1, v2, g_adj);
                    if (e.second) {
                        g_adj[e.first].minDist = 2;
                    } else {
                        abort();
                    }
                }
            }
        }
    }

    //setup available edges graph, Initially you can connect to anyone outside your partition.
    for (int i = 0; i < k; ++i) {
        for (int j = 0; j < i; ++j) {
            assert (i != j);

            for (int v: V[j]) {
                add_edge(X[i], v, available);
                assert(!edge(X[i], v, g).second);
            }

            for (int v: V[i]) {
                add_edge(X[j], v, available);
                assert(!edge(X[j], v, g).second);
            }

            add_edge(X[i], X[j], available);
            assert(!edge(X[i], X[j], g).second);

            for (int v1: V[i]) {
                for (int v2: V[j]) {
                    add_edge(v1, v2, available);
                    assert(!edge(v1, v2, g).second);
                }
            }
        }
    }


    cout << num_edges(available);

    for (int i = 0; i < n; ++i) {
        cout << i << ' ' << out_degree(i, available) << endl;
    }
    {
        ofstream outputfile(boost::lexical_cast<std::string>(std::time(nullptr)) + "s.dot");

        outputGraph(outputfile, g);
    }
    random_device rd;
    vector<pair<Graph::vertex_descriptor,Graph::vertex_descriptor>> toRemove;

    while(num_edges(available) > 0){
        auto ea = random_edge(available, rd);
        auto v1 = source(ea, available);
        auto v2 = target(ea, available);

        cout << v1 << ' ' << v2 << ' ';

        add_edge(v1, v2, g);

        //update distances in new graph
        for (auto e : iterator_range<adjGraph::edge_iterator>(edges(g_adj))){
            auto vn1 = target(e, g_adj);
            auto vn2 = source(e, g_adj);

            int dOld = g_adj[e].minDist;

            if ((vn1 != v1) && (vn2 != v1) && (vn1 != v2) && (vn2 != v2)) {
                auto evn1v1 = edge(vn1, v1, g_adj);
                auto evn1v2 = edge(vn1, v2, g_adj);
                auto evn2v1 = edge(vn2, v1, g_adj);
                auto evn2v2 = edge(vn2, v2, g_adj);

                assert(evn1v1.second && evn1v2.second && evn2v1.second && evn2v2.second);

                int dvn1v1 = g_adj[evn1v1.first].minDist;
                int dvn1v2 = g_adj[evn1v2.first].minDist;
                int dvn2v1 = g_adj[evn2v1.first].minDist;
                int dvn2v2 = g_adj[evn2v1.first].minDist;
                g_adj[e].minDist = min(dOld, min(dvn1v1 + dvn2v2 + 1, dvn2v1 + dvn1v2 + 1));
            } else if ((vn1 == v1) && (vn2 != v2) ){
                auto evn2v2 = edge(vn2,v2,g_adj);

                assert(evn2v2.second);

                g_adj[e].minDist = min(dOld, 1 + g_adj[evn2v2.first].minDist);
            } else if ((vn1 == v2) && (vn2 != v1)) {
                auto evn2v1 = edge(vn2,v1,g_adj);

                assert(evn2v1.second);

                g_adj[e].minDist = min(dOld, 1 + g_adj[evn2v1.first].minDist);
            } else if ((vn2 == v1) && (vn1 != v2)) {
                auto evn1v2 = edge(vn1,v2,g_adj);

                assert(evn1v2.second);

                g_adj[e].minDist = min(dOld, 1 + g_adj[evn1v2.first].minDist);
            } else if ((vn2 == v2) && (vn1 != v1)) {
                auto evn1v1 = edge(vn1,v1,g_adj);

                assert(evn1v1.second);

                g_adj[e].minDist = min(dOld, 1 + g_adj[evn1v1.first].minDist);
            } else if (((vn1 == v1) && (vn2 == v2))||((vn1 == v2) && (vn2 == v1))){
                g_adj[e].minDist = 1;
            } else {
                abort();
            }
        }

        //remove edges that would make the graph girth minG - 1 from available.
        for (auto e : iterator_range<Graph::edge_iterator>(edges(available))){
            auto vn1 = source(e, available);
            auto vn2 = target(e, available);

            auto en = edge(vn1, vn2, g_adj);

            assert(en.second);
            if (g_adj[en.first].minDist < (minG)){
                toRemove.push_back(make_pair(vn1,vn2));
            }
        }
        for (auto p: toRemove){
            remove_edge(p.first, p.second, available);
        }
        toRemove.clear();

        cout << " Edges: " << num_edges(g) << " Available Remaining: " << num_edges(available) << endl;
    }
    {
        ofstream outputfile(boost::lexical_cast<std::string>(std::time(nullptr)) + "e.dot");

        outputGraph(outputfile, g);
    }

    return 0;
}