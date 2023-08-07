#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <map>
using namespace std;

typedef vector<int> Fila;
typedef vector<Fila> Graph;

int main() {
    int n;
    while (cin >> n) {
        map<string, int> mapeado_a_numero;
        map<int, string> mapeado_a_string;
        string entrada;
        for (int i = 0; i < n; ++i) {
            cin >> entrada;
            mapeado_a_numero.insert(make_pair(entrada, i));
            mapeado_a_string.insert(make_pair(i, entrada));
        }
        int m;
        cin >> m;
        vector<int> apuntados(n, 0);
        Graph grafo(n);
        string u, v;
        for (int i = 0; i < m; ++i) {
            cin >> u >> v;
            grafo[mapeado_a_numero[u]].push_back(mapeado_a_numero[v]);
            ++apuntados[mapeado_a_numero[v]];
        }
        priority_queue<string, vector<string>, greater<string>> cola;
        auto it = mapeado_a_numero.begin();
        while (it != mapeado_a_numero.end()) {
            if (apuntados[it->second] == 0) {
                cola.push(it->first);
            }
            ++it;
        }
        queue<string> cola_aux;
        while (not cola.empty()) {
            string u = cola.top();
            cola_aux.push(u);
            cola.pop();
            for (int v : grafo[mapeado_a_numero[u]]) {  
                if (--apuntados[v] == 0) {
                    cola.push(mapeado_a_string[v]);
                }
            }
        }
        if (cola_aux.size() != n) {
            cout << "NO VALID ORDERING" << endl;
        }
        else {
            while (not cola_aux.empty()) {
                cout << cola_aux.front();
                cola_aux.pop();
            }
            cout << endl;
        }
    }
}