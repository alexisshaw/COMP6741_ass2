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
    const int num_vertices = 1000;
    const int num_sets = 10;
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
    for (int m = 0, dencity = none_edges.size()/50; m < dencity; m++) {
        int p = rand() % none_edges.size();
        pair<int, int> edgie = none_edges[p];
        int vx = edgie.first;
        int vy = edgie.second;
        pair<int, int> alterEgo( { vy, vx });
        if (find(edges.begin(), edges.end(), alterEgo) != edges.end()) { //if alter ego created at some point
            none_edges.erase(none_edges.begin() + p); //remove edge from set
            continue; // don't care about it
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
                edgie = {vy,vn}; //the hypotetical edge
                if (find(edges.begin(), edges.end(), edgie) != edges.end()) { //is there actually an edge from vy to vn
                    edgie = {vn,vx}; //a hypothetical link back home
                    if (find(edges.begin(), edges.end(), edgie)
                            != edges.end()) { //if vn points back to vx girth = 3, ignored
                        foundOne = false;
                        continue;
                    } else { //check vn's neighbourhood for girth = 4 links
                        for (int g = 0; g < setSize; g++) { //potential neighbourhood of vn
                            int vk = seti[g];//potentional neighbour of vn
                            edgie = {vn,vk};
                            if (find(edges.begin(), edges.end(),
                                    edgie) != edges.end()) { //there's actual an edge from vn to vk
                                edgie = {vk,vx};
                                if(find(edges.begin(),edges.end(),edgie) != edges.end()) { //if vk points back to vx girth = 4, ignored
                                    foundOne = false;
                                    continue;
                                } else { //should be guarantied a girth >= 5 here
                                    foundOne = true;
                                    continue;
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
        } else {
        }
        none_edges.erase(none_edges.begin() + p);
    }
    cout << endl << "all the none edges in the graph after selection= "
            << none_edges.size() << endl;
    cout << "all the edges in the final graph (subset)= " << edges.size()
            << endl << endl;

    vector<pair<int, int> > none_ex_edges;
    cout << "edges between exes: " << ex_edges.size() << endl;
    for (int i = 0, excount = exes.size(); i < excount; i++) {
        for (int j = 0; j < excount; j++) {
            int xi = exes[i];
            int xj = exes[j];
            if (xi == xj) {
                continue;
            }
            pair<int, int> edge( { xi, xj });
            pair<int, int> alter_ego( { xj, xi });
            if (find(ex_edges.begin(), ex_edges.end(), edge)
                    != ex_edges.end()) { //already there
                continue;
            }
            if (find(ex_edges.begin(), ex_edges.end(), alter_ego)
                    != ex_edges.end()) { //girth = 2
                continue;
            }
            none_ex_edges.push_back(edge);
        }
    }
    cout << "viable none edges between exes after generation: "
            << none_ex_edges.size() << endl;
    for (int m = 0, dencity = none_ex_edges.size(); m < dencity; m++) {
        int p = rand() % none_ex_edges.size();
        pair<int, int> edgie = none_ex_edges[p];
        int vx = edgie.first;
        int vy = edgie.second;
        pair<int, int> alterEgo( { vy, vx });
        if (find(ex_edges.begin(), ex_edges.end(), alterEgo)
                != ex_edges.end()) { //if alter ego created at some point
            none_ex_edges.erase(none_ex_edges.begin() + p); //remove edge from set
            continue; // don't care about it
        } else {
            vector<int> seti = exes;
            for (int n = 0, setSize = seti.size(); n < setSize; n++) { //y's neighbourhood
                int vn = seti[n]; //potential neighbour of vy
                edgie = {vy,vn}; //the hypotetical edge
                if (find(ex_edges.begin(), ex_edges.end(), edgie)
                        != ex_edges.end()) { //is there actually an edge from vy to vn
                    edgie = {vn,vx}; //a hypothetical link back home
                    if (find(ex_edges.begin(), ex_edges.end(), edgie)
                            != ex_edges.end()) { //if vn points back to vx girth = 3, ignored
                        foundOne = false;
                        continue;
                    } else { //check vn's neighbourhood for girth = 4 links
                        for (int g = 0; g < setSize; g++) { //potential neighbourhood of vn
                            int vk = seti[g];//potentional neighbour of vn
                            edgie = {vn,vk};
                            if (find(ex_edges.begin(), ex_edges.end(),
                                    edgie) != ex_edges.end()) { //there's actual an edge from vn to vk
                                edgie = {vk,vx};
                                if(find(ex_edges.begin(),ex_edges.end(),edgie) != ex_edges.end()) { //if vk points back to vx girth = 4, ignored
                                    foundOne = false;
                                    continue;
                                } else { //should be guarantied a girth >= 5 here
                                    foundOne = true;
                                    continue;
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
            ex_edges.push_back( { vx, vy });
            foundOne = false;
        } else {
        }
        none_ex_edges.erase(none_ex_edges.begin() + p);
    }

    cout << endl << "none edges between exes after selection: "
            << none_ex_edges.size() << endl;
    cout << "edges between exes final: " << ex_edges.size() << endl;

    cout << endl << "total edges in final graph: " << edges.size() << endl;

    cout << endl << "this be good" << endl; // prints this be good


    ofstream outputfile(boost::lexical_cast<std::string>(std::time(nullptr))+".dot");
    outputGraph(outputfile,  buildGraph(num_vertices, edges ));
    return 0;
}