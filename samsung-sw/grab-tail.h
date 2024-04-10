#include "types.h"

#define R first
#define C second

using namespace std;

/*
n * n ����, 3�� '�̻�' �� ��. ���� ��� - ..(�߰� �ּ� �Ѹ�).. - �Ӹ� ���
�־��� �̵� ���� ���󼭸� �̵��ϰ� (�� ���� �̵����� ���� �̾����ֵ�.), �̵� ���� ��ġ�� �ʴ´�.

1. �Ӹ� ����� ���� ��ĭ �̵�
2. �� ���帶�� ���� ������ ���� ���� ������
    ���� -> ������ (1 ~ n)
    �Ʒ� -> �� (n+1 , 2n)
    �� -> �� (2n+1, 3n)
    �� -> �Ʒ�(3n+1, 4n)
3. ���� �������� ���� ����� ������, '����' �� ������ ����� ���� ��� ������ ����. (�Ӹ����(1��°)���κ��� k��° ����̸� k^2)
    ���� ȹ���� ���� ������ �ٷ� �ٲ��. (�Ӹ��� ����, ������ ���ο� �Ӹ�)


�Է� : 0 - ��ĭ, 1 - �Ӹ� ��� , 2- �߰� ���, 3 - ���� ��� , 4-�̵���
��� : k���� ���� ���� �� ���� ��ԵǴ� ������ ����
*/

int n, m, k;
int board[21][21] = { {0,} };
int dirs[4][2] = { {1,0}, {0,1},{0,-1}, {-1,0} };
int score = 0;
int rounds = 1;

bool isOut(int nr, int nc) {
    return nr > n || nc > n || nr < 1 || nc < 1;
}

void move(pair<int, int> head, vector<vector<int>>& vis) {
    int backNo = 1;
    pair<int, int> back = head;

    int tempBoard[21][21] = { 0, };
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }

    while (backNo != 3) { // �ϴ� 1,2 ���� �����̰� ����
        pair<int, int> dest;

        int tempNo;
        pair<int, int> tempBack;

        for (auto& dir : dirs) {
            int nr = back.R + dir[0];
            int nc = back.C + dir[1];

            if (isOut(nr, nc)) continue; // ���� ��
            if (board[nr][nc] == 0) continue; // ���� ���� ��.

            if (backNo == 1 && board[nr][nc] == 3) {
                board[back.R][back.C] = 4; // ���� �ִ� ���� 4�� ��ŷ
                tempBoard[nr][nc] = backNo; // ���� �༱���� �Űܳ���
                vis[nr][nc] = 1;
            }
            else {
                if (board[nr][nc] == 4) {
                    //������ ��
                    board[back.R][back.C] = 4; // ���� �ִ� ���� 4�� ��ŷ
                    tempBoard[nr][nc] = backNo; // ���� �༱���� �Űܳ���
                    if (backNo == 1) vis[nr][nc] = 1;
                }
                else if (board[nr][nc] == 2) {
                    // �� �ڿ� �ִ� �� ���� ������Ʈ
                    tempNo = board[nr][nc];
                    tempBack = { nr, nc };
                }
                else {
                    tempNo = 3;
                    tempBoard[back.R][back.C] = 3;
                    if (tempBoard[nr][nc] == 3) tempBoard[nr][nc] = 4;
                    tempBack = { nr, nc };
                }
            }
        }

        backNo = tempNo;
        back = tempBack;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            board[i][j] = tempBoard[i][j];
        }
    }
}

vector<int> bfs(vector<int> start, int target) {
    vector<int> onePos;
    queue<vector<int>> que;
    que.push(start);

    vector<vector<int>> vis(n + 1, vector<int>(n + 1, 0));
    vis[start[0]][start[1]] = 1;

    while (!que.empty()) {
        vector<int> now = que.front();
        que.pop();

        if (board[now[0]][now[1]] == target) {
            onePos = now;
            break;
        }

        for (auto& dir : dirs) {
            int nr = now[0] + dir[0];
            int nc = now[1] + dir[1];
            int cost = now[2] + 1;

            if (isOut(nr, nc)) continue;
            if (vis[nr][nc]) continue;
            if (target == 1 && board[now[0]][now[1]] == 2 && board[nr][nc] == 3) continue;
            if (target == 1 && board[now[0]][now[1]] == 3 && board[nr][nc] == 1) continue;
            if (board[nr][nc] == 0 || board[nr][nc] == 4) continue;

            vis[nr][nc] = 1;
            que.push({ nr, nc, cost });
        }
    }

    return onePos;
}

pair<int, int> throwBall() {
    pair<int, int> now;
    vector<int> dir;

    // ������ �� ���� ���ϱ�
    if (rounds <= n) {
        now.R = rounds;
        now.C = 1;
        dir = { 0,1 };
    }
    else if (rounds > n && rounds <= 2 * n) {
        now.R = n;
        now.C = rounds - n;
        dir = { -1,0 };
    }
    else if (rounds > 2 * n && rounds <= 3 * n) {
        now.R = (n + 1) - (rounds - (2 * n));
        now.C = n;
        dir = { 0,-1 };
    }
    else if (rounds > 3 * n && rounds <= 4 * n) {
        now.R = 1;
        now.C = (n + 1) - (rounds - (3 * n));
        dir = { 1,0 };
    }

    // �� ������
    pair<int, int> target{ 0,0 };

    while (!isOut(now.R, now.C)) {
        if (board[now.R][now.C] > 0 && board[now.R][now.C] < 4) {
            // ��Ҵ� ���
            target = { now.R, now.C };
            break;
        }
        now.R += dir[0];
        now.C += dir[1];
    }

    if (target.first == 0 && target.second == 0) return { 0,0 }; // �ǰݴ����� ����

    if (board[target.R][target.C] == 1) {
        score += 1;
        return target;
    }
    else { // 2�� 3�� �¾��� ��
        auto res = bfs(vector<int>{target.R, target.C, 1}, 1);
        score += res[2] * res[2];
        return { res[0], res[1] }; // 1�� ��ġ
    }
}

void headToTail(pair<int, int> head) {
    auto res = bfs(vector<int>{head.R, head.C, 1}, 3);
    swap(board[res[0]][res[1]], board[head.R][head.C]);
}


int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    // ���⿡ �ڵ带 �ۼ����ּ���.
    cin >> n >> m >> k;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> board[i][j];
        }
    }

    while (k-- > 0) {
        // 1. �� ���� �Ӹ� ã�� - ��Ž
        vector<vector<int>> vis(n + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (board[i][j] == 1) {
                    // 1�� ã�Ҵٸ� �ùķ��̼� ����.
                    if (vis[i][j] == 1) continue;
                    move({ i, j }, vis);
                }
            }
        }

        // 2. �� ������
        pair<int, int> head = throwBall();
        if (head.first == 0 && head.second == 0) {
            rounds++;
            if (rounds > 4 * n) rounds = 1;
        }
        else {
            headToTail(head);
            rounds++;
            if (rounds > 4 * n) rounds = 1;
        }
    }

    cout << score << "\n";

    return 0;
}
