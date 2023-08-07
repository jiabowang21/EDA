#include <iostream>
#include <vector>
using namespace std;

// Merge two sorted subvectors: v[l...m], v[m+1...r]
int merge (vector<int>& v, int l, int m, int r){
	int i = l;
	int j = m + 1;
	vector<int> aux(r - l + 1);
	int k = 0;
	int inv = 0;
	while (i <= m and j <= r) {
		if (v[i] <= v[j]) {
			aux[k] = v[i];
			++i;
			++k;
		}
		else {
			inv += (m - i + 1);
			aux[k] = v[j];
			++j;
			++k;
		}
	}
	while (i <= m) {
		aux[k] = v[i];
		++i;
		++k;
	}
	while (j <= r) {
		aux[k] = v[j];
		++j;
		++k;
	}
	for (uint p = 0; p < aux.size(); ++p) v[l + p] = aux[p];
	return inv;
}

int merge_sort (vector<int>& v, int l, int r) {
	if (l >= r) return 0; // zero or one elements
	else {
		int m = (l + r)/2;
		return merge_sort(v, l, m) + merge_sort(v, m + 1, r) + merge(v, l, m, r);
	}
}


int merge_sort (vector<int>& v) {
	return merge_sort(v, 0, v.size()-1);
}

int main() {
	int n;
	while(cin >> n) {
		vector<int> v(n);
		for (auto& elem : v) cin >> elem;
		cout << merge_sort(v) << endl;
	}
}
