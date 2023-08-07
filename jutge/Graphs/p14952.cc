#include <iostream>
#include <vector>
#include <list>
#include <queue>
using namespace std;

typedef vector<int> Fila;
typedef vector<Fila> Graph;

int main() {
    int n, m;
    while (cin >> n >> m) {
        Graph V(n);
        vector<int> lvl(n, 0);
        int x, y;
        for (int i = 0; i < m; ++i) {
            cin >> x >> y;
            V[x].push_back(y);
            ++lvl[y]; 
        }
        priority_queue<int, vector<int>, greater<int>> cola;
        for (int i = 0; i < n; ++i) {
            if (lvl[i] == 0) cola.push(i);
        }
        list<int> list;
        while (not cola.empty()) {
            int u = cola.top();
            list.push_back(u);
            cola.pop();
            int m = V[u].size();
            for (int i = 0; i < m; ++i) {
                int s = V[u][i];
                if (--lvl[s] == 0) cola.push(s);
            }
        }
        bool primer = true;
        for (auto it = list.begin(); it != list.end(); ++it) {
            if (primer) {
                cout << *it;
                primer = false;
            }
            else cout << ' ' << *it;
        }
        cout << endl;
    }
}