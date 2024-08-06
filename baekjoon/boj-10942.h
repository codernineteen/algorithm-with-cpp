#include <types.h>

/*
���� :
���� ȫ���̿� �Բ� �Ӹ���� ���̸� �غ����� �Ѵ�.

����, ȫ���̴� �ڿ��� N���� ĥ�ǿ� ���´�. �� ����, ��쿡�� ������ �� M�� �Ѵ�.

�� ������ �� ���� S�� E(1 �� S �� E �� N)�� ��Ÿ�� �� ������, S��° ������ E��° ���� ���� �Ӹ������ �̷������ �����, ���� �� ������ ���� �Ӹ�����̴� �Ǵ� �ƴϴٸ� ���ؾ� �Ѵ�.

���� ���, ȫ���̰� ĥ�ǿ� ���� ���� 1, 2, 1, 3, 1, 2, 1��� ����.

S = 1, E = 3�� ��� 1, 2, 1�� �Ӹ�����̴�.
S = 2, E = 5�� ��� 2, 1, 3, 1�� �Ӹ������ �ƴϴ�.
S = 3, E = 3�� ��� 1�� �Ӹ�����̴�.
S = 5, E = 7�� ��� 1, 2, 1�� �Ӹ�����̴�.
�ڿ��� N���� ���� M���� ��� �־����� ��, ����� ����� ���ϴ� ���α׷��� �ۼ��Ͻÿ�.

Ǯ�� :
1. ���̺� ���� : dp[i][j]�� i�ڸ����� j�ڸ������� ���ڰ� �Ӹ���� ������ �����ϴ����� ���� ���θ� 0�� 1�� �����Ѵ�.
2. ��ȭ�� : 
    2.1 i == j(�� �ڸ� ���� ���), �׻� �Ӹ�����̹Ƿ� dp[i][j] = 1
    2.2 j-i == 1(�� �ڸ� ���� ���), �Ϲ����� �Ӹ���� �Ǵ� �˰����� ��� -> dp[i][j] = isPalindrome(sequence, i, j)
    2.2 else, ��ȭ�� ���. dp[i][j] = dp[i+1][j-1] , �� sequence[i] == sequence[j]�� �� �� ���ڰ� ���ƾ���.
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
