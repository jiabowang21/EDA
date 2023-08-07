#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int, int> ArcP;
typedef vector<ArcP> Fila;
typedef vector<Fila> Graph;

void dijkstra(const Graph &V, int x, int y) {
    vector<bool> visited(V.size(), false);
    vector<int> distances(V.size(), -1);
    distances[x] = 0;
    vector<int> ways(V.size(), 0);
    ways[x] = 1;
    priority_queue<ArcP, vector<ArcP>, greater<ArcP>> cola;
    cola.push(make_pair(0, x));
    int node;
    while (not cola.empty()) {
        node = cola.top().second;
        cola.pop();
        if (not visited[node]) {
            visited[node] = true;
            for (int i = 0; i < V[node].size(); ++i) {
                ArcP aux = V[node][i];
                if ((distances[aux.second] == -1) or (distances[aux.second] > distances[node] + aux.first)) {
                    distances[aux.second] = distances[node] + aux.first;
                    ways[aux.second] = ways[node];
                    cola.push(make_pair(distances[aux.second], aux.second));
                }
                else if (distances[aux.second] == distances[node] + aux.first) ways[aux.second] += ways[node];
            }
        }
    }
    if (distances[y] == -1) cout << "no path from " << x << " to " << y << endl;
    else {
        cout << "cost " << distances[y] << ", " << ways[y] << " way(s)" << endl;
    }
}

int main() {
    int n, m;
    while (cin >> n >> m) {
        Graph V(n);
        int u, v, p;
        for (int i = 0; i < m; ++i) {
            cin >> u >> v >> p;
            V[u].push_back(make_pair(p, v));
        }
        int x, y;
        cin >> x >> y;
        dijkstra(V, x, y);
    }
}