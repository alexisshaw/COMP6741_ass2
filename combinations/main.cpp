//algorithm from On O(1) Time Algorithms for Combinatorial Generation, http://comjnl.oxfordjournals.org/content/44/4/292.full.pdf

#include <iostream>
#include <vector>

using namespace std;

void out(int n, vector<signed int> c){
    for (int i=1; i<=n; i++)
        cout << c[i] << ' ';
    cout << endl;
}


int main(int argc, char* argv[]){
    int t=0,  r=0, j=0, M0 = 0, Mj = 0, S = 0, n=0;

    cout << "input n, r" << endl;
    cin >> n >> r;

    vector<int> c((unsigned long)n+1),e((unsigned long)n+2), Last((unsigned long)n+2); //positive going array

    M0   = r - n;
    t    = n + 1;
    c[0] = 0;

    j = 0;
    do {
        j = j+1;
        c[j] = j;
        e[j] = j - 1;
        if ((j % 2) == 1) {
            Last[j] = M0 + j;
        } else {
            Last[j] = j + 1;
        }
    } while(j!= n);

    int i = 0;

    out(n,c);

    if(n < r) {
        do {
            i++;
            S= c[j];
            Mj = M0 + j;
            e[n+1] = n;

            if ((j%2) == 1){
                if(c[j] == Mj){
                    c[j] = c[j-1] + 1;
                    Last[j+1] = c[j]+1;
                } else {
                    c[j] = c[j] + 1;
                }
            } else {
                if(c[j] == (c[j-1] + 1)){
                    c[j] = Mj;
                } else {
                    Last[j+1] = c[j];
                    c[j] = c[j] - 1;
                }
            }

            if (c[j] == Last[j]) {
                Last[j] = S;
                e[j+1] = e[j];
                e[j] = j-1;
            }

            if ((j < n) && (c[j] == Mj)){
                t = j;
                j = e[t+1];
                e[t+1] = t;
            } else {
                if (t == j){
                    t++;
                }
                if (t < e[n+1]){
                    j = t;
                } else{
                    j = e[n+1];
                }
            }

            out(n,c);
        } while (j != 0);
    }
    i++;
    cout << i << endl;


    return 0;
}