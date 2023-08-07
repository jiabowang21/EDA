#include <iostream>
#include <vector>
using namespace std;

typedef vector<char> Fila;
typedef vector<Fila> Graph;

void dfs(Graph &V, vector<vector<bool>> &vist, int x, int y, char c) {
    if (x < 0 or y < 0 or x > V.size() or y > V[0].size()) return;
    if (not vist[x][y]) {
        vist[x][y] = true;
        if (V[x][y] == '#') return;
        if (V[x][y] == '.') V[x][y] = c;
        dfs(V, vist, x+1, y, c);
        dfs(V, vist, x-1, y, c);
        dfs(V, vist, x, y+1, c);
        dfs(V, vist, x, y-1, c);
    }
}

void dfs(Graph &V) {
    int n, m;
    n = V.size();
    m = V[0].size();
    vector<vector<bool>> vist(n, vector<bool>(m, false));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (V[i][j] != '#' and V[i][j] != '.' and not vist[i][j]) {
                char c = V[i][j];
                dfs(V, vist, i, j, c);
            } 
        }
    }
}

int main() {
    int n, m;
    while (cin >> n >> m) {
        Graph V(n, Fila(m));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                cin >> V[i][j];
            }
        }
        dfs(V);
        for (int i = 0; i < n; ++i) {
            for(int j = 0; j < m; ++j) {
                cout << V[i][j];
            }
            cout << endl;
        }
        cout << endl;
    }
}