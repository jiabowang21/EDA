#include <iostream>
#include <vector>
using namespace std;

typedef vector<char> Fila;
typedef vector<Fila> graph;

bool dfs (int f, int c, graph &V, int n, int m) {
    if (f < 0 or c < 0 or f >= n or c >= m) return false;
    else {
        if (V[f][c] == 't') {
            return true;
        }
        if (V[f][c] == 'X') {
            return false;
        }
        V[f][c] = 'X';
        return dfs(f-1, c, V, n, m) or dfs(f+1, c, V, n, m) or dfs(f, c-1, V, n, m) or dfs(f, c+1, V, n, m);
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    graph V(n,Fila(m));
    //lectura del tablero
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> V[i][j]; 
        }
    }
    int f, c;
    cin >> f >> c;
    bool cierto = dfs(f-1, c-1, V, n, m);
    if (cierto) cout << "yes" << endl;
    else cout << "no" << endl;
}