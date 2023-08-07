#include <iostream>
#include <vector>
using namespace std;

void backtrack(vector<int> &V, int n, int u, int i, int z, int x) { //z => numero de zeros, x => numero de unos
    if (z > n-u or x > u) return;
    else {
        if (i == n) {
            bool first = true;
            for (int j = 0; j < n; ++j) {
                if (first) {
                    first = false;
                    cout << V[j];
                }
                else {
                    cout << " " << V[j];
                }
            }
            cout << endl;
            return;
        }
        V[i] = 0;
        backtrack(V, n, u, i + 1, z + 1, x);
        V[i] = 1;
        backtrack(V, n, u, i + 1, z, x + 1);
    }
}

int main() {
    int n, u;
    cin >> n >> u;
    int z = 0;
    int x = 0;
    vector<int> V(n);
    backtrack(V, n, u, 0, z, x);
}