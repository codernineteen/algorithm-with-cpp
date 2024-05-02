#pragma once

#include "types.h"

using namespace std;

long long int dp[100001] = { 0, };

// 연속합
// sub problem : 현재 인덱스 까지 최대 연속합
// 점화식 : 
//  1. 현재 숫자를 이전 연속합에 더했을 때, 현재 숫자보다 작아지는 경우, dp[i] = num[i]
//  2. 그게 아니라면, dp[i] = dp[i-1] + num[i]; // 이전 최대 연속합에 현재 숫자를 더함
// 최대 연속합은 매 반복마다 구해야함.
int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n;
    cin >> n;


    cin >> dp[1];
    long long int maxi = dp[1];

    for (int i = 2; i <= n; i++) {
        long long int cur;
        cin >> cur;

        if (dp[i - 1] + cur < cur) {
            dp[i] = cur;
        }
        else {
            dp[i] = cur + dp[i - 1];
        }

        maxi = max(maxi, dp[i]);
    }

    cout << maxi << "\n";
}
