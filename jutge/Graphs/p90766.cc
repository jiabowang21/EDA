#include <iostream>
#include <vector>
using namespace std;

typedef vector<char> Fila;
typedef vector<Fila> Graph;

int dfs(int f, int c, Graph &V, int n, int m) {
    if (f < 0 or c < 0 or f >= n or c >= m) return 0;
    else {
        int coins = 0;
        if (V[f][c] == 't') coins = 1;
        if (V[f][c] == 'X') return 0;
        V[f][c] = 'X';
        return coins + dfs(f-1, c, V, n, m) + dfs(f+1, c, V, n, m) + dfs(f, c-1, V, n, m) + dfs(f, c+1, V, n, m); 
    }
}

int main() {
    int n, m;
    cin >> n >> m;
    Graph V(n,Fila(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> V[i][j];
        }
    }
    int f, c;
    cin >> f >> c;
    int result = dfs(f-1, c-1, V, n, m);
    cout << result << endl;
}