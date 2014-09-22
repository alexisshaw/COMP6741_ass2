//algorithm from On O(1) Time Algorithms for Combinatorial Generation, http://comjnl.oxfordjournals.org/content/44/4/292.full.pdf

#include <iostream>
#include <vector>
#include "combination.h"

using namespace std;

int main(int argc, char* argv[]){
    int i = 0;
    vector<char> c =  {'a','b','c','d','e','f'};

    for (auto comb : combination<char>(c, 4)){
        for(auto d : comb) {
            cout << d << ' ';
        }
        cout << ", "<< comb.lastOut() << "->" << comb.lastIn() << endl;
    }




    return 0;
}