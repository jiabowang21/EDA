#include <iostream>
#include <map>
using namespace std;

int main() {
  cout.setf(ios::fixed);
  cout.precision(4);
  map<int, int> num;
  string x;
  double total = 0;
  double i = 0;
  while (cin >> x) {
    auto it = num.begin();
    if (x == "number") {
      int n;
      cin >> n;
      it = num.find(n);
      if (it == num.end()) num.insert(make_pair(n, 1));
      else ++it->second;
      ++i;
      total += n;
    }
    else if (x == "delete") {
      if (!num.empty()) {
        total -= it->first;
        if (it->second == 1) num.erase(it->first);
        else --it->second;
        --i;
      }
    }
    if (num.empty()) {
      cout << "no elements" << endl;
    }
    else {
      auto last = num.end();
      --last;
      auto first = num.begin();
      cout << "minimum: " << first->first << ", maximum: " << last->first << ", average: " << total/i << endl;
    }
  }
}
