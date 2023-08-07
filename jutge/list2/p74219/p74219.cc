#include <iostream>
#include <vector>
using namespace std;
// Donades dues matriu A,B 2x2, retorna A*B mod m
vector<vector<int> > mult(const vector<vector<int> >& A, const vector<vector<int> >& B, int m) {
  return {
    {(A[0][0]*B[0][0] + A[0][1]*B[1][0])%m , (A[0][0]*B[0][1] + A[0][1]*B[1][1])%m },
    {(A[1][0]*B[0][0] + A[1][1]*B[1][0])%m , (A[1][0]*B[0][1] + A[1][1]*B[1][1])%m }
  };
}


vector<vector<int> > potencia (const vector<vector<int> >& aux2, int n, int m) {
  if (n == 1) return aux2;
  else {
    vector<vector<int>> Y = potencia(aux2, n/2, m);
    if (n%2 == 0) return mult(Y, Y, m);
    else return mult(mult(Y, Y, m), aux2, m);
  }
}

int fibonacci(int n, int m) {
	if (n < 2) return n;
	vector<vector<int>> aux({ { 1, 1 }, { 1, 0 } });
  vector<vector<int>> aux2 = potencia(aux, n, m);
	return aux2[0][1];
}

int main() {
	int n;
	long m;
	while (cin >> n >> m)
		cout << fibonacci(n, m) << endl;
}
