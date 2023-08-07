#include <iostream>
#include <vector>
using namespace std;

int fixed_points(const vector<int> &S, int nm, int esq, int dre) {
  if (esq > dre) return -1;
  int m = (dre + esq)/2;
  if ((S[m] + nm) > m) return fixed_points(S, nm, esq, m - 1);
  else if ((S[m] + nm) < m) return fixed_points(S, nm, m + 1, dre);
  else {
    if (m == 1 or (S[m - 1] + nm) != m -1) {
      return m;
    }
    else return fixed_points(S, nm, esq, m - 1);
  }
  return -1;
}

int main() {
  int count = 1;
  int n;
  while (cin >> n) {
    vector<int> S(n);
    for (int i = 1; i <= n; ++i) {
      cin >> S[i];
    }
    int m;
    cin >> m;
    cout << "Sequence #" << count << endl;
    for (int j = 0; j < m; ++j) {
      int nm;
      cin >> nm;
      int trobat = fixed_points(S, nm, 1, n);
      if (trobat > 0) cout << "fixed point for " << nm << ": " << trobat << endl;
      else cout << "no fixed point for " << nm << endl;
    }
    cout << endl;
    ++count;
  }
}
