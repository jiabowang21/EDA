#include <iostream>
#include <map>
using namespace std;

int main() {
  map<int, int> num;
  map<int, int>::iterator it;
  int x, y, n;
  bool found = false;
  while (cin >> x >> y >> n) {
    int pos = 1;
    num.insert(make_pair(n, 0));
    while (n <= 100000000 and not found){
      if (n%2 == 0) n = n / 2 + x;
      else n = 3 * n + y;
      it = num.find(n);
      if (it == num.end()) num.insert(make_pair(n, pos));
      else {
        found = true;
        n = pos - (it->second);
      }
      ++pos;
    }
    cout << n << endl;
    num.clear();
    found = false;
  }
}
