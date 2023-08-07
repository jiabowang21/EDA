#include <iostream>
#include <vector>
using namespace std;

typedef vector<char> C;
typedef vector<bool> P;

void transformar(C &consonants, C &vocals, int n, int c, int v, C &result, P &cons, P &vo) {
    if (c == n and v == n) {
        for (int i = 0; i < 2*n; ++i) {
            cout << result[i];
        }
        cout << endl;
        return;
    }
    else {
        //afegir consonant
        if (v == c) {
            for (int i = 0; i < n; ++i) {
                if (!cons[i]) {
                    cons[i] = true;
                    result[v+c] = consonants[i];
                    transformar(consonants, vocals, n, c+1, v, result, cons, vo);
                    cons[i] = false;
                }
            }
        }
        //afegir vocal
        else if (c > v) {
            for (int i = 0; i < n; ++i) {
                if (!vo[i]) {
                    vo[i] = true;
                    result[v+c] = vocals[i];
                    transformar(consonants, vocals, n, c, v+1, result, cons, vo);
                    vo[i] = false;
                }
            }
        }
    }
}

int main() {
    int n;
    cin >> n;
    C vocals(n);
    C consonants(n);
    C result(2*n);
    P cons(n, false);
    P vo(n, false);
    for (int i = 0; i < n; ++i) {
        cin >> consonants[i];
    }
    for (int i = 0; i < n; ++i) {
        cin >> vocals[i];
    }
    transformar(consonants, vocals, n, 0, 0, result, cons, vo);
}