#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef vector<int> Fila;
typedef vector<Fila> Graph;

bool is_cyclic (const Graph &V, int n) {
    Fila indeg(n, 0);
    for (int u = 0; u < n; ++u) {
        for (int v = 0; v < V[u].size(); ++v) {
            ++indeg[V[u][v]];
        }
    }
    Fila candidats;
    for (int u = 0; u < n; ++u) {
        if (indeg[u] == 0) {
            candidats.push_back(u);
        }
    }
    while (not candidats.empty()) {
        int u = candidats.back();
        candidats.pop_back();
        --n;
        for(int v : V[u]) {
            --indeg[v];
            if (indeg[v] == 0) {
                candidats.push_back(v);
            }
        }
    }
    return n > 0;
}

int main() {
    int n, m;
    while (cin >> n >> m) {
        Graph V(n);
        int x, y;
        for (int i = 0; i < m; ++i) {
            cin >> x >> y;
            V[x].push_back(y);
        }
        if (is_cyclic(V, n)) cout << "yes" << endl;
        else cout << "no" << endl;
    }
}