#include <iostream>
#include <set>
using namespace std;

int main() {
  set <string> cjt;
  string w;
  cin >> w;
  if (w != "END") {
    cjt.insert(w);
    cout << w << endl;
    auto mid = cjt.begin();
    while (cin >> w and w != "END") {
      cjt.insert(w);
      if (w < *mid and cjt.size()%2 == 0) --mid;
      else if (w > *mid and cjt.size()%2 != 0) ++mid;
      cout << *mid << endl;
    }
  }
}
