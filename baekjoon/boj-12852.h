#pragma once

#include "types.h"

int dp[1000000] = { 0, };

void solve() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N;
	cin >> N;

	dp[1] = 0;
	unordered_map<int, int> m;

	for (int i = 2; i <= N; i++) {
		int minusOne = dp[i - 1];
		int divTwo = i % 2 == 0 ? dp[i / 2] : INT_MAX;
		int divThree = i % 3 == 0 ? dp[i / 3] : INT_MAX;

		dp[i] = min(minusOne, min(divTwo, divThree)) + 1;

		if (minusOne == dp[i]-1) m[i] = i-1;
		else if(divTwo == dp[i]-1) m[i] = i / 2;
		else m[i] = i / 3;
	}

	cout << dp[N] << "\n";
	while (m.find(N) != m.end()) {
		cout << N << " ";
		N = m[N];
	}
	cout << 1;

}