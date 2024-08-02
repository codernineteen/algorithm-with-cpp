/*

���� : 
LCS(Longest Common Subsequence, ���� ���� �κ� ����)������ �� ������ �־����� ��, ����� �κ� ������ �Ǵ� ���� �� ���� �� ���� ã�� �����̴�.

���� ���, ACAYKP�� CAPCAK�� LCS�� ACAK�� �ȴ�.

Ǯ�� :
1. ���̺� ���� : dp[i][j]�� ù��° ���ڿ��� i��° ����, �ι�° ���ڿ��� j��° ���ڱ����� �ִ� ���� �κ� ���ڿ��� ���̸� �����Ѵ�.
2. ��ȭ�� ���� : dp[i][j] = max(dp[i-1][j], dp[i][j-1], dp[i-1][j-1] + equality) , ���⼭ equality�� ���� i,j �� ������ �� 1�� ����, �׷��� ������ 0�� ���� ������.
��ȭ�Ŀ��� ������ ���� �̹� ���̳� ���� ��ġ�� i�� j�� ���¿��� ���� ����� ���� ����̴�.
������ i-1, j-1�� ��쿡�� i, j�� ���� ������ �ݿ����� �ʾ����Ƿ� equality�� ���� 1�� ������ �� �ִ�.
*/

#pragma once

#include "types.h"
int dp[1002][1002] = { {0,} };

using namespace std;

int solve() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	std::string s1, s2;
	cin >> s1 >> s2;
	
	int n = s1.size(), m = s2.size();

	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			int eq = s1[i-1] == s2[j-1] ? 1 : 0;

			dp[i][j] = max(max(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1] + eq);
		}
	}

	cout << dp[n][m] << "\n";

	return 0;
}
