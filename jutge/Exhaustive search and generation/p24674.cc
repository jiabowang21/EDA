#include <iostream>
#include <vector>
using namespace std;

void backtrack(const vector<string> &V, vector<string> &V2, vector<bool> &vist, int n, int i) {
    if (i == n) {
        cout << '(';
        for (int j = 0; j < n; ++j) {
            cout << (j == 0 ? "" : ",") << V2[j]; 
        }
        cout << ')' << endl;
        return;
    }
    for (int j = 0; j < n; ++j) {
        if (!vist[j]) {
            vist[j] = true;
            V2[i] = V[j];
            backtrack(V, V2, vist, n, i + 1);
            vist[j] = false;
        }
    }
}

int main() {
    int n;
    cin >> n;
    vector<string> V(n);
    vector<string> V2(n);
    vector<bool> vist(n, false);
    for (int i = 0; i < n; ++i) {
        cin >> V[i];
    }
    backtrack(V, V2, vist, n, 0);
}