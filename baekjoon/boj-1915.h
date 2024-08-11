#include <types.h>
/*
���� :
n��m�� 0, 1�� �� �迭�� �ִ�. �� �迭���� 1�� �� ���� ū ���簢���� ũ�⸦ ���ϴ� ���α׷��� �ۼ��Ͻÿ�.

0	1	0	0
0	1	1	1
1	1	1	0
0	0	1	0

���� ���� ���������� ����� 2��2 �迭�� ���� ū ���簢���̴�.

Ǯ�� :
���簢���� �����Ǵ� ������ � ���Ҹ� �ѷ��� �׵θ��� ��� 1�̻��̸� ���� ���� ���� ���̴�.
���� ���, �� ���� ���̰� 3�� ���簢���� i, j���Ҹ� �������� ���� 2�� ���簢���� ������ �� ���� ���ΰ� �־�� �Ѵ�.

1. ���̺� ���� : dp[i][j]�� (i,j)��° ���Ұ� 1�� �� �ش� ���Ҹ� �ѷ��� �׵θ��� �ּ� ���� + 1
2. ��ȭ�� ���� : dp[i][j] = (grid[i][j] == 1) ? min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]) + 1 : grid[i][j];

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
