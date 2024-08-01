#pragma once
#include "types.h"

#define u64 uint64_t

using namespace std;

/*
���� :
45656�̶� ���� ����.
�� ���� ������ ��� �ڸ��� ���̰� 1�̴�. �̷� ���� ��� ����� �Ѵ�.
N�� �־��� ��, ���̰� N�� ��� ���� �� �� �� �ִ��� ���غ���. 0���� �����ϴ� ���� ��ܼ��� �ƴϴ�.

Ǯ�� :
- ���̺� ���� : dp[i][j] �� ���̰� i�̰� ���ڸ��� j�� ������ ��� ������ ������
- ��ȭ�� ���� : 
	1. j�� 0�̳� 9�� �ƴ� �� ) dp[i][j] = dp[i-1][j-1] + dp[i-1][j+1] 
	2. j�� 0�� �� )  dp[i][j] = dp[i][j+1]
	3. j�� 9�� �� )  dp[i][j] = dp[i][j-1]
*/

int solve() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N;
	cin >> N;

	u64 dp[101][10] = { 0, };

	// �ʱⰪ
	dp[1][0] = 0;
	for (int i = 1; i <= 9; i++)
	{
		dp[1][i] = 1;
	}

	for (int i = 2; i <= N; i++)
	{
		for (int j = 0; j <= 9; j++)
		{
			if (j != 0 && j != 9)
			{
				dp[i][j] = (dp[i - 1][j + 1] + dp[i - 1][j - 1]) % 1000000000;
			}
			else if (j == 0)
			{
				dp[i][j] = dp[i - 1][j + 1] % 1000000000;
			}
			else
			{
				dp[i][j] = dp[i - 1][j - 1] % 1000000000;
			}
		}
	}

	u64 res = 0;
	for (auto& n : dp[N]) res += n;
	res %= 1000000000;

	cout << res << "\n";

	return 0;
}
