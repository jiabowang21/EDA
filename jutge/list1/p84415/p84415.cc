#include <iostream>
#include <map>
using namespace std;

int main() {
  map<string, int> bossa;
  string x;
  while (cin >> x) {
    if (x == "minimum?") {
      if (bossa.empty()) {
        cout << "indefinite minimum" << endl;
      }
      else {
        auto it = bossa.begin();
        cout << "minimum: " << it->first << ", " << it->second << " time(s)" << endl;
      }
    }
    else if (x == "maximum?") {
      if (bossa.empty()) {
        cout << "indefinite maximum" << endl;
      }
      else {
        auto it = bossa.end();
        --it;
        cout << "maximum: " << it->first << ", " << it->second << " time(s)" << endl;
      }
    }
    else if (x == "store") {
      string p;
      cin >> p;
      ++bossa[p];
    }
    else {
      string p;
      cin >> p;
      auto it = bossa.find(p);
      if (it != bossa.end() and --it->second == 0) {
        bossa.erase(p);
      }
    }
  }
}
