#pragma once

/*
[문제]

상근이와 선영이가 다른 사람들이 남매간의 대화를 듣는 것을 방지하기 위해서 대화를 서로 암호화 하기로 했다. 그래서 다음과 같은 대화를 했다.

상근: 그냥 간단히 암호화 하자. A를 1이라고 하고, B는 2로, 그리고 Z는 26으로 하는거야.
선영: 그럼 안돼. 만약, "BEAN"을 암호화하면 25114가 나오는데, 이걸 다시 글자로 바꾸는 방법은 여러 가지가 있어.
상근: 그렇네. 25114를 다시 영어로 바꾸면, "BEAAD", "YAAD", "YAN", "YKD", "BEKD", "BEAN" 총 6가지가 나오는데, BEAN이 맞는 단어라는건 쉽게 알수 있잖아?
선영: 예가 적절하지 않았네 ㅠㅠ 만약 내가 500자리 글자를 암호화 했다고 해봐. 그 때는 나올 수 있는 해석이 정말 많은데, 그걸 언제 다해봐?
상근: 얼마나 많은데?
선영: 구해보자!
어떤 암호가 주어졌을 때, 그 암호의 해석이 몇 가지가 나올 수 있는지 구하는 프로그램을 작성하시오.

[풀이]

- 테이블 정의 : dp[i]는 현재 주어진 숫자에서 i번째 자리에 위치할 때, 가능한 해독 가짓수를 저장한다.
- 점화식 정의 :
    - len == 1일때 : dp[0] = 1, dp[1]은 첫번째 자리가 0이 아니면 1, 0이면 0
    - len > 1일때, dp[len]는 한 단계 전(onestep)과 두 단계 전(twostep)에 의해 결정
        - onestep은 현재 자리의 숫자가 0이 아니라면 dp[len-1]의 값. 아니면 0
        - twostep은 현재 자리 숫자와 이전 자리 숫자로 합성한 두 자리 숫자의 크기가 26과 10사이면 dp[len-2]값. 아니면 0
          여기서 중요한 것은 10이하의 숫자도 걸러야 하는 것인다. 왜냐하면 01과 1은 엄연히 다르기 때문에 전자의 경우 해독할 방법이 없다.

*/

#include "types.h"

using namespace std;

int dp[5001] = { 0, };

int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    string num;
    cin >> num;

    int prev = -1;
    for (int i = 0; i < num.size(); i++)
    {
        int digit = num[i] - '0'; // char to int

        if (i == 0) 
        {
            dp[1] = digit != 0 ? 1 : 0;
            dp[0] = 1;
            prev = digit;
            continue;
        }

        int len = i + 1;
        int comp = digit + prev * 10;

        int oneStep = (digit != 0) ? dp[len - 1] : 0;
        int twoStep = (comp <= 26 && comp  >= 10) ? dp[len - 2] : 0;
        dp[len] = (oneStep + twoStep) % 1000000;
        prev = digit;
    }

    cout << dp[num.size()] << "\n";

    return 0;
}
