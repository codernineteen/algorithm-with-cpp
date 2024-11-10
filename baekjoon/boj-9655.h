#include <types.h>

using namespace std;

int dp[1001][1001] = { {0,} };
int grid[1001][1001] = { {0,} };


int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n,m;
    cin >> n >> m;

    for (int i = 0; i < n; i++)
    {
        string t;
        cin >> t;

        for (int j = 0; j < m; j++)
        {
            grid[i][j] = t[j] - '0';
        }
    }


    for (int i = 0; i < n; i++) dp[i][0] = grid[i][0];
    for (int i = 0; i < m; i++) dp[0][i] = grid[0][i];

    int res = 0;
    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            int adj = min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]);
            if (adj == grid[i][j]) dp[i][j] = adj + 1;
            else dp[i][j] = grid[i][j];
            
            res = max(dp[i][j], res);
        }
    }

    cout << res * res << "\n";
    

    return 0;
}
