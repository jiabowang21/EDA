#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

typedef vector<int> Fila;

void backtrack(const Fila &V, int n, int s, int sum, int x, Fila &f) {
    if (sum == s and x == n) {
        cout << "{";
        bool first = true;
        for (int i = 0; i < f.size(); ++i) {
            if (first) {
                first = false;
                cout << f[i];
            }
            else cout << "," << f[i];
        }
        cout << "}" << endl;
    }
    else if (x < n) {
        f.push_back(V[x]);
        backtrack(V, n, s, sum+V[x], x+1, f);
        f.pop_back();
        backtrack(V, n, s, sum, x+1, f);
    }
}

int main() {
    int s, n;
    cin >> s >> n;
    Fila V(n);
    Fila f;
    for (int i = 0; i < n; ++i) {
        cin >> V[i];
    }
    backtrack(V, n, s, 0, 0, f);
}