#include <iostream>
#include <vector>

using namespace std;
// Donades dues matriu A,B 2x2, retorna A*B mod m
vector<vector<int>> mult(
			 const vector<vector<int>>& A,
			 const vector<vector<int>>& B,
			 int m
			 ) {
  return {
    {(A[0][0]*B[0][0] + A[0][1]*B[1][0])%m , (A[0][0]*B[0][1] + A[0][1]*B[1][1])%m },
      {(A[1][0]*B[0][0] + A[1][1]*B[1][0])%m , (A[1][0]*B[0][1] + A[1][1]*B[1][1])%m }};
}


vector<vector<int>> potencia (const vector<vector<int>>& M, int n, int m) {
  if (n == 0) return {{1,0},{0,1}};
  else {
    vector<vector<int>> Y = potencia(M,n/2,m);
    if (n%2 == 0) return mult(Y,Y,m);
    else return mult(mult(Y,Y,m),M,m);
  }
}

int main(){
  // Llegeixo dades i calculo
  vector<vector<int>> M(2,vector<int>(2));
  while (cin >> M[0][0]) {
    int n, m;
    cin >> M[0][1] >> M[1][0] >> M[1][1] >> n >> m;
    vector<vector<int>> P = potencia(M,n,m);
    cout << P[0][0] << " " << P[0][1] << endl;
    cout << P[1][0] << " " << P[1][1] << endl;
    cout << string(10,'-') << endl;
  }

}
