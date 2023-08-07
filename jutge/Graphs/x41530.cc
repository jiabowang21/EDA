#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> Fila;
typedef vector<Fila> Graph;

void dfs_rec(const Graph &V, int n, vector<bool> &vist) {
    if (not vist[n]) {
        vist[n] = true;
        for (int i = 0; i < V[n].size(); ++i) {
            int p = V[n][i];
            dfs_rec(V, p, vist);
        }
    }
}

int dfs(const Graph &V, int n) {
    int result = 0;
    vector<bool> vist(n, false);
    for (int i = 0; i < n; ++i) {
        if (not vist[i]) ++result;
        dfs_rec(V, i, vist);
    }
    return result;
}

int main() {
    int n, m;
    while (cin >> n >> m) {
        Graph V(n);
        int x, y;
        for (int i = 0; i < m; ++i) {
            cin >> x >> y;
            V[x].push_back(y);
            V[y].push_back(x);
        }
        int result = dfs(V, n);
        if (n - result == m) cout << result << endl;
        else cout << "no" << endl;
    }
}