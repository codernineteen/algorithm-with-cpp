#pragma once

#include "types.h"

 int memo[2000003] = { 0, };

void solve_11568() {
    ios::sync_with_stdio(0);
    cin.tie(0);
    int N;
    cin >> N;

    while (N-- > 0) {
        int num;
        cin >> num;
        memo[num + 1000000]++;
    }
    for (int i = 2000002; i >= 0; i--) {
        for (int j = 0; j < memo[i]; j++) {
            cout << i - 1000000 << "\n";
        }
    }
}