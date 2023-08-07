#include <iostream>
#include <map>
#include <set>
using namespace std;

struct compare {
  bool operator() (const pair<string, int> &a, const pair<string, int> &b) const {
    if (a.second == b.second) return a.first < b.first;
    else return a.second > b.second;
  }
};

int main() {
  map<string, int> jugadors;
  set<string> estat;
  string x;
  while (cin >> x) {
    string nom;
    cin >> nom;
    auto jestat = jugadors.find(nom);
    if (x == "LOGIN") {
      auto iestat = estat.find(nom);
      if (iestat == estat.end()) estat.insert(nom);
      if (jestat == jugadors.end()) jugadors.insert(make_pair(nom, 1200));
    }
    else if (x == "LOGOUT") {
      auto iestat = estat.find(nom);
      if (iestat != estat.end()) estat.erase(nom);
    }
    else if (x == "PLAY") {
      auto iestat = estat.find(nom);
      string nom2;
      cin >> nom2;
      auto iestat2 = estat.find(nom2);
      auto jestat2 = jugadors.find(nom2);
      if (iestat == estat.end() or iestat2 == estat.end()) {
        cout << "jugador(s) no connectat(s)" << endl;
      }
      else {
        jestat->second += 10;
        if (jestat2->second > 1200) jestat2->second -= 10;
      }
    }
    else if (x == "GET_ELO") {
      cout << nom << " " << jestat->second << endl;
    }
  }
  cout << endl << "RANKING" << endl;
  set<pair<string, int>, compare> pls;
  for (auto jp = jugadors.begin(); jp != jugadors.end(); ++jp) {
    pls.insert(make_pair(jp->first, jp->second));
  }
  for (auto js = pls.begin(); js != pls.end(); ++js) {
    cout << (*js).first << " " << (*js).second << endl;
  }
}
