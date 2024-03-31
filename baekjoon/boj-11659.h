#pragma once

#include "types.h"

using namespace std;

int dp[100001] = { 0, };
int num[100001] = { 0, };

int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int N;
    int M;
    cin >> N >> M;

    for (int i = 1; i <= N; i++) {
        cin >> num[i];
    }

    dp[1] = num[1];
    for (int i = 2; i <= N; i++) {
        dp[i] = dp[i - 1] + num[i]; // 현재 지점까지 누적합을 갱신
    }

    while (M-- > 0) {
        int a, b;
        cin >> a >> b;
        cout << dp[b] - dp[a-1] << "\n"; // (b 지점 까지의 누적합 - a 지점 전까지의 누적합 ) == a ~ b 사이의 구간 합
    }

    return 0;
}
