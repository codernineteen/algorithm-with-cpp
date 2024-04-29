#include <types.h>

using namespace std;

int dp[41][2] = { {0,} };


int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    dp[1][1] = 1;
    dp[0][0] = 1;

    int N;
    cin >> N;
    while (N-- > 0) {
        int M;
        cin >> M;

        for (int i = 2; i <= M; i++) {
            dp[i][0] = dp[i - 1][0] + dp[i - 2][0];
            dp[i][1] = dp[i - 1][1] + dp[i - 2][1];
        }

        cout << dp[M][0] << " " << dp[M][1] << "\n";
    }
}
