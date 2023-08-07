#include <iostream>
#include <map>
using namespace std;

int main() {
    int n;
    while (cin >> n) {
        map<string, int> m;
        int totalGames = 0;
        for (int i = 0; i < n; ++i) {
            string game;
            cin >> game;
            ++m[game];
            ++totalGames;
        }
        int g;
        cin >> g;
        for (int i = 0; i < g; ++i) {
            string game;
            cin >> game;
            if (m[game] + 1 <= totalGames - m[game]) {
                ++m[game];
                ++totalGames;
            }
        }
        for (auto& g:m) cout << g.first << " " << g.second << endl;
        cout << string(20, '-') << endl;
    }
}