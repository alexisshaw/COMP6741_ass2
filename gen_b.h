#include <vector>
#include <random>
#include <algorithm>


inline vector<int> getX(const int n, const int k){
    //get partitions
    vector<int> vertex_descriptors(n-k);
    iota(vertex_descriptors.begin(), vertex_descriptors.end(), 0);

    random_device rd;

    shuffle(vertex_descriptors.begin(), vertex_descriptors.end(), rd);

    vector<int> x(k);

    sort(vertex_descriptors.begin(), vertex_descriptors.begin() + k-1);

    x[0] = 0;
    for (int i=1; i < k; i++){
        x[i] = vertex_descriptors[i-1] + i;
    }
    return x;
}

inline vector<int> getY(vector<int> X){
    vector<int> Y(X.size());

    for(int i=0; i < X.size(); i++){
        Y[i] = X[i] + 1;
    }
    return Y;
}

inline vector<vector<int>> getV(vector<int> X, int n){
    vector<vector<int>> V (X.size());
    for(int i=0; i < X.size(); i++){
        int limit = 0;
        if ((i+1) < X.size()){
            limit = X[i+1];
        } else {
            limit = n;
        }

        for (int j = X[i] + 2 ; j < limit; ++j){
            V[i].push_back(j);
        }
    }
    return V;
}