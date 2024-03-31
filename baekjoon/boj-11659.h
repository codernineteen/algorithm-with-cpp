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
        dp[i] = dp[i - 1] + num[i]; // ���� �������� �������� ����
    }

    while (M-- > 0) {
        int a, b;
        cin >> a >> b;
        cout << dp[b] - dp[a-1] << "\n"; // (b ���� ������ ������ - a ���� �������� ������ ) == a ~ b ������ ���� ��
    }

    return 0;
}
