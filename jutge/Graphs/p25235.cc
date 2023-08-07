#include <iostream>
#include <vector>
#include <queue>
#include <stack>
using namespace std;

typedef pair<int, int> ArcP; //pes primer, node segon
typedef vector<ArcP> Fila;
typedef vector<Fila> Graph;

void dijkstra(const Graph &V, int x, int y) {
    vector<bool> visited(V.size(), false);
    vector<int> distances(V.size(), -1);
    distances[x] = 0;
    vector <int> steps(V.size(), 0);
    priority_queue<ArcP, vector<ArcP>, greater<ArcP>> cola;
    cola.push(make_pair(0, x));
    int node;
    while (not cola.empty() and cola.top().second != y) {
        node = cola.top().second;
        cola.pop();
        if (not visited[node]) {
            visited[node] = true;
            for (int i = 0; i < V[node].size(); ++i) {
                ArcP aux = V[node][i];
                if ((distances[aux.second] == -1) or (distances[aux.second] > distances[node] + aux.first)) {
                    distances[aux.second] = distances[node] + aux.first;
                    steps[aux.second] = steps[node] + 1;
                    cola.push(make_pair(distances[aux.second], aux.second));
                }
                else if ((distances[aux.second] == distances[node] + aux.first) and steps[aux.second] > steps[node]+1) steps[aux.second] = steps[node]+1; 
            }
        }
    }
    if (cola.empty()) {
        cout << "no path from " << x << " to " << y << endl;
        return;
    }
    cout << "cost " << distances[y] << ", " << steps[y] << " step(s)" << endl;
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