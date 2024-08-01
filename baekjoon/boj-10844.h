#pragma once
#include "types.h"

#define u64 uint64_t

using namespace std;

/*
문제 :
45656이란 수를 보자.
이 수는 인접한 모든 자리의 차이가 1이다. 이런 수를 계단 수라고 한다.
N이 주어질 때, 길이가 N인 계단 수가 총 몇 개 있는지 구해보자. 0으로 시작하는 수는 계단수가 아니다.

풀이 :
- 테이블 정의 : dp[i][j] 는 길이가 i이고 끝자리가 j로 끝나는 계단 수들의 누적합
- 점화식 정의 : 
	1. j가 0이나 9가 아닐 때 ) dp[i][j] = dp[i-1][j-1] + dp[i-1][j+1] 
	2. j가 0일 때 )  dp[i][j] = dp[i][j+1]
	3. j가 9일 때 )  dp[i][j] = dp[i][j-1]
*/

int solve() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N;
	cin >> N;

	u64 dp[101][10] = { 0, };

	// 초기값
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
