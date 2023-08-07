#include <iostream>
#include <vector>
using namespace std;

typedef vector<bool> h;
typedef vector<int> p;

int diagonal_asc(int i, int j) {
    return i + j;
}

int diagonal_desc(int n, int i, int j) {
    return i - j + n - 1;
}

void n_reines(h &mc, h &diag_asc, h &diag_desc, int n, int i, p &reines) {
    if (i == n) {
        for (int j = 0; j < n; ++j) {
            int x = reines[j];
            for (int k = 0; k < n; ++k) {
                if (k == x) cout << "Q";
                else cout << ".";
            }
            cout << endl;
        }
        cout << endl;
        return;
    }
    else {
        for (int j = 0; j < n; ++j) {
            if (!mc[j] and !diag_asc[diagonal_asc(i, j)] and !diag_desc[diagonal_desc(n, i, j)]) {
                reines[i] = j;
                mc[j] = diag_asc[diagonal_asc(i, j)] = diag_desc[diagonal_desc(n, i, j)] = true;
                n_reines(mc, diag_asc, diag_desc, n, i + 1, reines);
                mc[j] = diag_asc[diagonal_asc(i, j)] = diag_desc[diagonal_desc(n, i, j)] = false;
            }
        }
    }
}

int main() {
    int n; 
    cin >> n;
    h mc(n, false);
    h diag_asc(2*n-1, false);
    h diag_desc(2*n-1, false);
    p reines(n);
    n_reines(mc, diag_asc, diag_desc, n, 0, reines);
}