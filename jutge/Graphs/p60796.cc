#include <iostream>
#include <vector>
#include <queue>
#include <list>
using namespace std;

typedef vector<char> Fila;
typedef vector<Fila> Graph;

int bfs (const Graph &V, int n, int m, int x1, int y1) {
    vector <vector <bool> > vist(n, vector<bool>(m, false));
    queue <pair<pair<int, int>, int> > q;
    q.push(make_pair(make_pair(x1-1, y1-1), 0));
    int x, y, d;
    while (not q.empty()) {
        x = (q.front().first).first;
        y = (q.front().first).second;
        d = (q.front().second);
        q.pop();
        if (V[x][y] == 't') {
            return d;
        }
        else if (not vist[x][y]) {
            vist[x][y] = true;
            if (x > 0 and V[x-1][y] != 'X') q.push(make_pair(make_pair(x-1, y), d+1));
            if (x < n - 1 and V[x+1][y] != 'X') q.push(make_pair(make_pair(x+1, y), d+1));
            if (y > 0 and V[x][y-1] != 'X') q.push(make_pair(make_pair(x, y-1), d+1));
            if (y < m - 1 and V[x][y+1] != 'X') q.push(make_pair(make_pair(x, y+1), d+1));
        }
    }
    return -1;
}

int main() {
    int n, m;
    cin >> n >> m;
    Graph V(n, Fila(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> V[i][j];
        }
    }
    int x, y;
    cin >> x >> y;
    int valor = -1;
    valor = bfs(V, n, m, x, y);
    if (valor != -1) cout << "distancia minima: " << valor << endl;
    else cout << "no es pot arribar a cap tresor" << endl;
}