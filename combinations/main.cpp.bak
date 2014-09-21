#include <iostream>
#include <vector>


using namespace std;

void out(int n, vector<signed int> q){
    for (int i=1; i<=n; i++)
        cout << q[i] << ' ';
    cout << endl;
}


int main(int argc, char* argv[]){
    cout << "hello world" << endl;

    int i=0, n=0, r=0;

    cout << "input n, r" << endl;
    cin >> n >> r;

    vector<signed int> d(n+2);        //take +/- 1;
    vector<signed int> a(n+1),q(n+1),solve(n+1),up(n+1),down(n+1),pos(n+1); //positive going array

    for (i=1; i <= n ; i++){
        a[i] = i;
        q[i] = i;
        up[i] = i;
        down[i] = i;
        solve[i] = i;
        pos[i] = i;
        d[i] = 1;
    }
    d[n+1] = i;
    i = n;

    do{
        out(n, q);
        if(d[i+1] < 0){
            q[pos[a[i]]] = a[i] + d[i];
            pos[a[i] + d[i]] = pos[a[i]];
        } else if (d[i] > 0){
            q[pos[a[i]]] = a[solve[i]] + d[i];
            pos[a[solve[i]]+d[i]] = pos[a[i]];
        } else {
            q[pos[a[solve[i]]]] = a[i] + d[i];
            pos[a[i]+d[i]] = pos[a[solve[i]]];
        }

        a[i] = a[i] + d[i];

        if (d[i+1] > 0){
            a[solve[i]] = a[solve[i]] + d[i];
        }

        up[i] = i;

        if (((d[i] > 0) && (a[i] == (r-n+i))) || ((d[i] < 0) && (a[i] == (a[i-1] + 1)))){
            up[i] = up[i-1];
            up[i-1] = i-1;
            down[up[i]] = i;

            if (d[i] < 0){
                solve[up[i]] = i;
            }

            d[i] = -d[i];

            if ((d[i] < 0) || (i == n)){
                i = up[i];
            } else {
                i = down[i];
            }
        }

    }while(i != 0);
    out(n, q);

    return 0;
}