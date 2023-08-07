#include <iostream>
#include <vector>
#include <queue>
using namespace std;

typedef pair<int, int> ArcP; //pes primer, node segon
typedef vector<ArcP> Fila;
typedef vector<Fila> Graph;

int dijkstra(const Graph &V, int x, int y) {
    vector<bool> visited(V.size(), false);
    vector<int> distances(V.size(), -1);
    distances[x] = 0;
    priority_queue<ArcP, vector<ArcP>, greater<ArcP> > cola;
    cola.push(make_pair(0, x));
    int actual_node;
    while (!cola.empty()) {
        actual_node = cola.top().second;
        cola.pop();
        if (!visited[actual_node]) {
            visited[actual_node] = true;
            for (int i = 0; i < V[actual_node].size(); ++i) {
                ArcP aux = V[actual_node][i];
                if ((distances[aux.second] == -1) or distances[aux.second] > distances[actual_node] + aux.first) {
                    distances[aux.second] = distances[actual_node] + aux.first;
                    cola.push(make_pair(distances[aux.second], aux.second));
                }
            }
        }
    }
    return distances[y];
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
        int result = dijkstra(V, x, y);
        if (result == -1) cout << "no path from " << x << " to " << y << endl;
        else cout << result << endl;
    }
}