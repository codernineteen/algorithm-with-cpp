#pragma once

#include "types.h"

using namespace std;

long long int dp[100001] = { 0, };

// ������
// sub problem : ���� �ε��� ���� �ִ� ������
// ��ȭ�� : 
//  1. ���� ���ڸ� ���� �����տ� ������ ��, ���� ���ں��� �۾����� ���, dp[i] = num[i]
//  2. �װ� �ƴ϶��, dp[i] = dp[i-1] + num[i]; // ���� �ִ� �����տ� ���� ���ڸ� ����
// �ִ� �������� �� �ݺ����� ���ؾ���.
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
