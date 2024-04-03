#include "types.h"

using namespace std;

int solve() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N, K;
	cin >> N >> K;

	vector<int> coins;
	for (int i = 0; i < N; i++) {
		int value;
		cin >> value;
		coins.push_back(value);
	}

	int res = 0;
	for(auto it = coins.rbegin(); it != coins.rend(); it++) {
		res += K / *it;
		K %= *it;
	}

	cout << res << "\n";
	return 0;
}
