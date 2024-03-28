//정수 X에 사용할 수 있는 연산은 다음과 같이 세 가지 이다.
//
//X가 3으로 나누어 떨어지면, 3으로 나눈다.
//X가 2로 나누어 떨어지면, 2로 나눈다.
//1을 뺀다.
//정수 N이 주어졌을 때, 위와 같은 연산 세 개를 적절히 사용해서 1을 만들려고 한다.연산을 사용하는 횟수의 최솟값을 출력하시오.
//
//입력
//첫째 줄에 1보다 크거나 같고, 106보다 작거나 같은 정수 N이 주어진다.
//
//출력
//첫째 줄에 연산을 하는 횟수의 최솟값을 출력한다.

#include "types.h"

int dp[1000000] = { 0, };

void solve() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N;
	cin >> N;

	dp[1] = 0;
	for (int i = 2; i <= N; i++) {
		int minusOne = dp[i - 1];
		int divTwo = i % 2 == 0 ? dp[i / 2] : INT_MAX;
		int divThree = i % 3 == 0 ? dp[i / 3] : INT_MAX;

		dp[i] = min(minusOne, min(divTwo, divThree)) + 1;
	}

	cout << dp[N] << "\n";

}