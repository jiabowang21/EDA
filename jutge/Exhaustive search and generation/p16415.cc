#include <iostream>
#include <vector>
using namespace std;

vector<int> t;
// mc[j] si ja hi ha reina a la columna j,
// md1[k] si ja hi ha reina a la diagonal i+j = k, etc.
vector<bool> mc, md1, md2;

//diagonal ascendent
int diag1(int i, int j) { 
    return i + j; 
}

//diagonal descendent
int diag2(int n, int i, int j) { 
    return i - j + n - 1; 
}

int reines(int n, int i) {
    if (i == n) {
        return 1;
    }
    else {
        int res = 0;
        for (int j = 0; j < n; ++j) {
            //no hi ha reina a la columna j i no hi ha reina a la diagonal i, j
            if (not mc[j] and not md1[diag1(i, j)] and not md2[diag2(n, i, j)]) {
                mc[j] = md1[diag1(i, j)] = md2[diag2(n, i, j)] = true;
                res += reines(n, i + 1);
                mc[j] = md1[diag1(i, j)] = md2[diag2(n, i, j)] = false;
            }
        }
        return res;
    }
}

int main() {
    int n;
    cin >> n;
    mc = vector<bool>(n, false);
    md1 = md2 = vector<bool>(2*n-1, false);
    cout << reines(n, 0) << endl;
}