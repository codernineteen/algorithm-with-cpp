/*

문제 : 
LCS(Longest Common Subsequence, 최장 공통 부분 수열)문제는 두 수열이 주어졌을 때, 모두의 부분 수열이 되는 수열 중 가장 긴 것을 찾는 문제이다.

예를 들어, ACAYKP와 CAPCAK의 LCS는 ACAK가 된다.

풀이 :
1. 테이블 정의 : dp[i][j]는 첫번째 문자열의 i번째 문자, 두번째 문자열의 j번째 문자까지의 최대 누적 부분 문자열의 길이를 저장한다.
2. 점화식 정의 : dp[i][j] = max(dp[i-1][j], dp[i][j-1], dp[i-1][j-1] + equality) , 여기서 equality는 문자 i,j 가 동일할 때 1의 값을, 그렇지 않으면 0의 값을 가진다.
점화식에서 주이할 점은 이미 행이나 열의 위치가 i나 j인 상태에서 이전 결과를 보는 경우이다.
오로지 i-1, j-1인 경우에만 i, j에 대한 정보가 반영되지 않았으므로 equality에 따라 1을 더해줄 수 있다.
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
