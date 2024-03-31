#include "types.h"

using namespace std;

int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int N;
    cin >> N;

    int dp[1001] = { 0, };
    dp[0] = 0;
    dp[1] = 1;
    dp[2] = 2;

    for (int i = 3; i <= N; i++) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    cout << dp[N] << "\n";

    return 0;
}
