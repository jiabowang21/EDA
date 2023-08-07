#include <iostream>
#include <vector>
using namespace std;

typedef vector<int> Fila;
typedef vector<Fila> Graph;

bool dfs(vector<int> &colors, const Graph &V, int k){
    for (int i = 0; i < V[k].size(); ++i) {
        if (colors[V[k][i]] < 0) {
            colors[V[k][i]] = 1 - colors[k]; 
            if (not dfs(colors, V, V[k][i])) return false;
        }
        else if (colors[V[k][i]] == colors[k]) {
            return false;
        }
    }
    return true;
}

bool dfs (const Graph &V) {
    vector<int> colors(V.size(), -1);
    for (int i = 0; i < V.size(); ++i) {
        if (colors[i] < 0) {
            colors[i] = 1;
            if (not dfs(colors, V, i)) return false;
        }
    }
    return true;
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
        if (dfs(V)) cout << "yes" << endl;
        else cout << "no" << endl;
    }
}