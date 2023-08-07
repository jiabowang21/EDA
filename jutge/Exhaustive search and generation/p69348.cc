#include <iostream>
#include <vector>
using namespace std;

void backtrack(vector<int> &V, vector<bool> &vist, int n, int i) {
    if (i == n) {
        cout << '(';
        for (int j = 0; j < n; ++j) {
            if (j != n - 1) cout << V[j] << ",";
            else cout << V[j]; 
        }
        cout << ')' << endl;
        return;
    }
    for (int j = 0; j < n; ++j) {
        if (not vist[j+1]) {
            vist[j + 1] = true;
            V[i] = j+1;
            backtrack(V, vist, n, i + 1);
            vist[j + 1] = false;
        }
    }
}

int main() {
    int n;
    cin >> n;
    vector<int> V(n);
    vector<bool> vist(n, false);
    backtrack(V, vist, n, 0);
}