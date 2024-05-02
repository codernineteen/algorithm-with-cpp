#pragma once

#include "types.h"

using namespace std;

long long int dp[91] = { 0, };

int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    long long int n;
    cin >> n;

    dp[0] = 1;
    dp[1] = 1;
    dp[2] = 1;

    for (int i = 3; i <= n; i++) {
        for (int j = i - 2; j >= 0; j--) {
            dp[i] += dp[j];
        }
    }

    cout << dp[n] << "\n";
}
