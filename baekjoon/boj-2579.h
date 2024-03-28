#pragma once

#include "types.h"

using namespace std;

int solve() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N;
	cin >> N;
	
	int dp[301][3] = { {0,0,0} };
	dp[0][0] = 0;
	cin >> dp[1][1];

	for (int i = 2; i <= N; i++) {
		int score;
		cin >> score;
		
		dp[i][1] = max(dp[i - 2][1], dp[i - 2][2]) + score;
		dp[i][2] = dp[i - 1][1] + score;
	}

	cout << max(dp[N][1], dp[N][2]) << "\n";
	return 0;
}