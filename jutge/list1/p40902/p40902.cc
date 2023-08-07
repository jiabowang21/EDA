#include <iostream>
#include <map>
using namespace std;

int main() {
  map<string, int> jugadors;
  string nom;
  while (cin >> nom) {
    string op;
    cin >> op;
    auto it = jugadors.find(nom);
    if (op == "enters") {
      if (it == jugadors.end()) {
        jugadors.insert(make_pair(nom, 0));
      }
      else {
        cout << nom << " is already in the casino" << endl;
      }
    }
    else if (op == "wins") {
      int n;
      cin >> n;
      if (it == jugadors.end()) {
        cout << nom << " is not in the casino" << endl;
      }
      else {
        it->second += n;
      }
    }
    else {
      if (it == jugadors.end()) {
        cout << nom << " is not in the casino" << endl;
      }
      else {
        cout << nom << " has won " << it->second << endl;
        jugadors.erase(nom);
      }
    }
  }
  cout << "----------" << endl;
  auto it = jugadors.begin();
  while (it != jugadors.end()) {
    cout << it->first << " is winning " << it->second << endl;
    ++it;
  }
}
