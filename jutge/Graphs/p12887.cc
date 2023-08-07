#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int, int> ArcP;
typedef vector<ArcP> Fila;
typedef vector<Fila> Graph;

void Algorisme_prim(const Graph &V) {
    vector<bool> vis(V.size(), false);
    priority_queue<ArcP, vector<ArcP>, greater<ArcP>> Q;
    Q.push(make_pair(0, 0));
    int sum = 0;
    while (!Q.empty()) {
        int weight = Q.top().first;
        int vertex = Q.top().second;
        Q.pop();
        if (!vis[vertex]) {
            vis[vertex] = true;
            sum += weight;
            for (int i = 0; i < V[vertex].size(); ++i) {
                ArcP p = V[vertex][i];
                int next_w = p.first;
                int next_v = p.second;
                Q.push(make_pair(next_w, next_v));
            }
        }
    }
    cout << sum << endl;
}

int main() {
    int n, m;
    while (cin >> n >> m) {
        Graph V(n);
        int u, v, p;
        for (int i = 0; i < m; ++i) {
            cin >> u >> v >> p;
            V[u-1].push_back(make_pair(p, v-1));
            V[v-1].push_back(make_pair(p, u-1));
        }
        Algorisme_prim(V);
    }
}