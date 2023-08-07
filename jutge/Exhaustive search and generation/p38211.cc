#include <iostream>
#include <vector>
using namespace std;

typedef vector<bool> h;
typedef vector<int> p;

void reines(h &mc, h &d1, h &d2, p &t, int n, int i, bool &trobat) {
    if (i == n) {
        for (int j = 0; j < n; ++j) {
            int x = t[j];
            for (int k = 0; k < n; ++k) {
                if (k == x) cout << 'Q';
                else cout << '.';
            }
            cout << endl;
        }
        trobat = true;
        return;
    }
    else {
        for (int j = 0; j < n; ++j) {
            if (!mc[j] and !d1[i+j] and !d2[i-j+n-1]) {
                t[i] = j;
                mc[j] = d1[i+j] = d2[i-j+n-1] = true;
                reines(mc, d1, d2, t, n, i+1, trobat);
                mc[j] = d1[i+j] = d2[i-j+n-1] = false;
                if (trobat) return;
            }
        }
    }
}

int main() {
    int n;
    cin >> n;
    h mc(n, false);
    h d1(n, false);
    h d2(n, false);
    p t(n);
    bool trobat = false;
    reines(mc, d1, d2, t, n, 0, trobat);
    if (not trobat) cout << "NO SOLUTION" << endl;
}