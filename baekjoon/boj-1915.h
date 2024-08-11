#include <types.h>
/*
문제 :
n×m의 0, 1로 된 배열이 있다. 이 배열에서 1로 된 가장 큰 정사각형의 크기를 구하는 프로그램을 작성하시오.

0	1	0	0
0	1	1	1
1	1	1	0
0	0	1	0

위와 같은 예제에서는 가운데의 2×2 배열이 가장 큰 정사각형이다.

풀이 :
정사각형이 형성되는 조건은 어떤 원소를 둘러싼 테두리가 모두 1이상이며 같은 값을 가질 때이다.
예를 들어, 한 변의 길이가 3인 정사각형은 i, j원소를 기준으로 길이 2의 정사각형이 인접한 두 변을 감싸고 있어야 한다.

1. 테이블 정의 : dp[i][j]는 (i,j)번째 원소가 1일 때 해당 원소를 둘러싼 테두리의 최소 값에 + 1
2. 점화식 정의 : dp[i][j] = (grid[i][j] == 1) ? min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1 : grid[i][j];

*/

using namespace std;

int dp[1001][1001] = { {0,} };
int grid[1001][1001] = { {0,} };


int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int n, m;
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

    int res = 0;
    for (int i = 0; i < n; i++) { res = max(grid[i][0], res); dp[i][0] = grid[i][0]; }
    for (int i = 0; i < m; i++) { res = max(grid[0][i], res); dp[0][i] = grid[0][i]; }


    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < m; j++)
        {
            dp[i][j] = (grid[i][j] == 1) ? min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1 : grid[i][j];
            res = max(dp[i][j], res);
        }
    }

    cout << res * res << "\n";


    return 0;
}
