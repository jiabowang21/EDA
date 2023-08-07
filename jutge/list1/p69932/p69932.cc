#include <iostream>
#include <sstream>
#include <set>
using namespace std;

int main() {
  string s;
  while (getline(cin, s)) {
    set<int> S;
    stringstream ss(s);
    int x;
    while(ss >> x) S.insert(x);
    int res = 0;
    if (!S.empty()) {
      auto it = S.begin();
      int first = *it;
      ++it;
      ++res;
      while (it != S.end()) {
        if(first%2 != *it%2) {
          ++res;
        }
        first = *it;
        ++it;
      }
    }
    cout << res << endl;
  }
}
