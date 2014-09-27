//============================================================================
// Name        : GraphGenerator.cpp
// Author      : Sindre S. Fjermestad
// Version     :
// Copyright   : My copyright notice
// Description : Generator for graphs of a certain type
//============================================================================

#include <iostream>
#include <list>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include <boost/lexical_cast.hpp>

#include "kernelisation/graph_serialization.h"

using namespace std;

int main() {
        const int num_vertices = 35;
        const int num_sets = 5;
        const int dencity_factor = 1;
        vector<int> vertices;
        vector<vector<int> > sets;
        //1.PARTITIONING
        int min = 0;
        int max = num_vertices;
        srand(time(0));
        for (int i = 0; i < num_vertices; i++) {
                vertices.push_back(i);
        }
        for (int i = 0; i < num_sets; i++) {
                vector<int> set;
                sets.push_back(set);
        }
        for (int i = 0; i < num_vertices; i++) {
                int p = min + (rand() % (int) ((max - i) - min));
                sets[i % num_sets].push_back(vertices[p]);
                vertices.push_back(vertices[p]);
                vertices.erase(vertices.begin() + p);
        }
        //2.IN SUBSET SELECTION
        vector<int> exes; //pushing back will make the index here be the corresponding set
        //HERE IS WHERE BOOST::GRAPH COMES IN!
        vector<pair<int, int> > edges;
        vector<pair<int, int> > ex_edges;
        for (int i = 0; i < num_sets; i++) {
                int p = (rand() % (sets[i].size()));
                int xi = sets[i][p];
                exes.push_back(xi);
                //3.INSIDE/OUTSIDE SUBSET CONNECTION
                for (int j = 0; j < sets[i].size(); j++) {
                        int yi = sets[i][j];
                        if (sets[i][j] != sets[i][p]) {
                                edges.push_back( { yi, xi });
                        }
                }
        }
        //4.SUBSET CONNECTION
        for (int i = 0; i < num_sets - 1; i++) {
                int p;
                if (num_sets >= 10) {
                        p = i + 1 + (rand() % 2);
                } else {
                        p = i + 1;
                }
                edges.push_back( { exes[i], exes[p] });
                ex_edges.push_back( { exes[i], exes[p] });
        }
        //5.FILLING THE VOID
        vector<pair<int, int> > none_edges;
        for (int i = 0; i < num_sets; i++) { //all sets
                vector<int> seti = sets[i];
                for (int x = 0, setSize = seti.size(); x < setSize; x++) { // all vertices in set
                        int vx = seti[x];
                        for (int y = 0; y < setSize; y++) { //all other vertices in set
                                int vy = seti[y];
                                pair<int, int> edge( { vx, vy });
                                if (find(edges.begin(), edges.end(), edge) != edges.end()) { // if edge not actaully a none edge a.k.a. an edge!
                                        continue;
                                }
                                if (edge.first == edge.second) { //if it's a self referencing edge
                                        continue;
                                }
                                edge = {vy,vx};
                                if (find(edges.begin(), edges.end(), edge) != edges.end()) { // if edge has an alter ego!
                                        continue;
                                }
                                none_edges.push_back( { vx, vy });
                        }
                }
        }
        cout << "all the none edges in the graph after generating them = "
                        << none_edges.size() << endl;
        cout << "all the edges in the graph = " << edges.size() << endl;
        bool foundOne = false;
        cout << endl << endl;
        for (int m = 0, dencity = none_edges.size() / dencity_factor; m < dencity;
                        m++) {
                foundOne = false;
                int p = rand() % none_edges.size();
                pair<int, int> edgie = none_edges[p];
                int vx = edgie.first;
                int vy = edgie.second;
                pair<int, int> alterEgo( { vy, vx });
                if (find(edges.begin(), edges.end(), alterEgo) != edges.end()) { //if alter ego created at some point
                        none_edges.erase(none_edges.begin() + p); //remove edge from set
                        continue; // don't care about its
                } else {
                        vector<int> seti;

                        for (int setsearch = 0, setcount = sets.size();
                                        setsearch < setcount; setsearch++) {
                                seti = sets[setsearch];
                                if (find(seti.begin(), seti.end(), vx) != seti.end()) {
                                        seti = sets[setsearch];
                                        break;
                                }
                        }
                        for (int n = 0, setSize = seti.size(); n < setSize; n++) { //y's neighbourhood
                                int vn = seti[n]; //potential neighbour of vy
                                if (vn == vy || vn == vx
                                                || find(exes.begin(), exes.end(), vn) != exes.end()) {
                                        continue;
                                }
                                edgie = {vy,vn}; //the hypotetical edge
                                if (find(edges.begin(), edges.end(), edgie) != edges.end()) { //is there actually an edge from vy to vn
                                        edgie = {vn,vx}; //a hypothetical link back home
                                        foundOne = false;
                                        if (find(edges.begin(), edges.end(), edgie)
                                                        != edges.end()) { //if vn points back to vx girth = 3, ignored
                                                continue;
                                        } else { //check vn's neighbourhood for girth = 4 links
                                                for (int k = 0; k < setSize; k++) { //potential neighbourhood of vn
                                                        int vk = seti[k];//potentional neighbour of vn
                                                        if (vk == vy || vk == vx
                                                                        || find(exes.begin(), exes.end(), vk) != exes.end()) {
                                                                continue;
                                                        }
                                                        edgie = {vn,vk};
                                                        if (find(edges.begin(), edges.end(),
                                                                                        edgie) != edges.end()) { //there's actual an edge from vn to vk
                                                                edgie = {vk,vx};
                                                                foundOne = false;
                                                                if(find(edges.begin(),edges.end(),edgie) != edges.end()) { //if vk points back to vx girth = 4, ignored
                                                                } else { //should be guarantied a girth >= 5 here
                                                                        foundOne = true;
                                                                }
                                                        } else {
                                                                foundOne = true;
                                                        }
                                                }
                                        }
                                } else {
                                        foundOne = true;
                                }
                        }
                }
                if (foundOne) {
                        edges.push_back( { vx, vy });
                        foundOne = false;
                }
                none_edges.erase(none_edges.begin() + p);
        }
        cout << endl;
        cout << endl << "all the none edges in the graph after selection= "
                        << none_edges.size() << endl;
        cout << "all the edges in the final graph (subset)= " << edges.size()
                        << endl << endl;
        int p = rand() % exes.size();
        while (p <= exes.size() / 4) { //just so it's not COMPLETELY the same ex "clique"
                p = rand() % exes.size();
        }
        for (int i = 0, excount = p; i < excount; i++) {
                for (int j = i + 1; j < exes.size() - rand() % 3; j++) {
                        pair<int, int> edge( { exes[i], exes[j] });
                        if (find(ex_edges.begin(), ex_edges.end(), edge)
                                        != ex_edges.end()) {
                        } else {
                                edges.push_back(edge);
                        }
                }
        }

        cout << "total edges in final graph: " << edges.size() << endl;

        cout << endl << "in the set you'll find: " << endl;

        for (int i = 0, setCount = sets.size(); i < setCount; i++) {
                cout << "(";
                for (int j = 0, setSize = sets[i].size(); j < setSize; j++) {
                        cout << sets[i][j] << ", ";
                }
                cout << ")" << endl;
        }
        cout << "with exes: ";
        for (int i = 0, exCount = exes.size(); i < exCount; i++) {
                cout << exes[i] << " ";
        }
        cout << endl << "and all the edges: " << endl;
        for (int i = 0, edgeCount = edges.size(); i < edgeCount; i++) {
                if (i > 22) {
                        if (i % num_sets == 0) {
                                cout << endl;
                        }
                        cout << "(" << edges[i].first << "," << edges[i].second << ") ";
                }
        }

        cout << endl << endl << "this be good" << endl; // prints this be good

    ofstream outputfile(boost::lexical_cast<std::string>(std::time(nullptr))+".dot");
    outputGraph(outputfile,  buildGraph(num_vertices, edges ));
    return 0;
}