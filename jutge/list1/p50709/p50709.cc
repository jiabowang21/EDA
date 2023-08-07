#include <iostream>
#include <queue>
using namespace std;

int main() {
  priority_queue <int> numeros;
  int n;
  char c;
  while (cin >> c) {
    if (c == 'S') {
      cin >> n;
      numeros.push(n);
    }
    else if (numeros.empty()) {
      if (c == 'D' or c == 'I') {
        cin >> n;
      }
      cout << "error!" << endl;
    }
    else if (c == 'A') {
      cout << numeros.top() << endl;
    }
    else if (c == 'R') {
      numeros.pop();
    }
    else {
      cin >> n;
      if (c == 'D') n = -n;
      n = numeros.top() + n;
      numeros.pop();
      numeros.push(n);
    }
  }
}
