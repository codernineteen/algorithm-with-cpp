#include <types.h>

/*
문제 :
명우는 홍준이와 함께 팰린드롬 놀이를 해보려고 한다.

먼저, 홍준이는 자연수 N개를 칠판에 적는다. 그 다음, 명우에게 질문을 총 M번 한다.

각 질문은 두 정수 S와 E(1 ≤ S ≤ E ≤ N)로 나타낼 수 있으며, S번째 수부터 E번째 까지 수가 팰린드롬을 이루는지를 물어보며, 명우는 각 질문에 대해 팰린드롬이다 또는 아니다를 말해야 한다.

예를 들어, 홍준이가 칠판에 적은 수가 1, 2, 1, 3, 1, 2, 1라고 하자.

S = 1, E = 3인 경우 1, 2, 1은 팰린드롬이다.
S = 2, E = 5인 경우 2, 1, 3, 1은 팰린드롬이 아니다.
S = 3, E = 3인 경우 1은 팰린드롬이다.
S = 5, E = 7인 경우 1, 2, 1은 팰린드롬이다.
자연수 N개와 질문 M개가 모두 주어졌을 때, 명우의 대답을 구하는 프로그램을 작성하시오.

풀이 :
1. 테이블 정의 : dp[i][j]는 i자리부터 j자리까지의 숫자가 팰린드롬 패턴을 구성하는지에 대한 여부를 0과 1로 저장한다.
2. 점화식 : 
    2.1 i == j(한 자리 수인 경우), 항상 팰린드롬이므로 dp[i][j] = 1
    2.2 j-i == 1(두 자리 수인 경우), 일반적인 팰린드롬 판단 알고리즘을 사용 -> dp[i][j] = isPalindrome(sequence, i, j)
    2.2 else, 점화식 사용. dp[i][j] = dp[i+1][j-1] , 단 sequence[i] == sequence[j]로 양 끝 숫자가 같아야함.
*/

using namespace std;

bool dp[2001][2001] = { {0,} };

bool isPalindrome(const vector<int>& seq, int s, int e)
{
    while (s < e)
    {
        if (seq[s] != seq[e]) return false;
        s++;
        e--;
    }

    return true;
}


int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    int N;
    cin >> N;

    vector<int> seq;
    seq.push_back(0);
    for (int i = 0; i < N; i++)
    {
        int n;
        cin >> n;
        seq.push_back(n);
    }

    for (int len = 1; len <= N; len++)
    {
        for (int s = 1; s <= N - len + 1; s++)
        {
            int e = s + len - 1;
            if (len == 1) dp[s][e] = 1;
            else if (len == 2) dp[s][e] = isPalindrome(seq, s, e);
            else { // len >= 3
                if (seq[s] != seq[e]) dp[s][e] = 0; // edge diff
                else dp[s][e] = dp[s + 1][e - 1]; // edge equal
            }
        }
    }

    int M;
    cin >> M;

    vector<pair<int, int>> que;
    for (int i = 0; i < M; i++)
    {
        int S, E;
        cin >> S >> E;

        cout << dp[S][E] << "\n";
    }

    return 0;
}
