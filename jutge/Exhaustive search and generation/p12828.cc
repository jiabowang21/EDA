#include <iostream>
#include <vector>
using namespace std;

void backtrack(vector<int> &V, int n, int i) {
    if (i == n) {
        bool first = true;
        for (int j = 0; j < n; ++j) {
            if (first) {
                first = false;
                cout << V[j];
            }
            else {
                cout << " " << V[j];
            }
        }
        cout << endl;
        return;
    }
    V[i] = 0;
    backtrack(V, n, i + 1);
    V[i] = 1;
    backtrack(V, n, i + 1);
}

int main() {
    int n;
    cin >> n;
    vector<int> V(n);
    backtrack(V, n, 0);
}

