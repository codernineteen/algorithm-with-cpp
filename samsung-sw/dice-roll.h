#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>

using namespace std;

#define R first
#define C second
/*
n x n ����, ���ֺ��� ������ ���� 7, ���� ���̰� 1 x 1 �� ������ü �ֻ���
1,1 ���� ���� , ó������ �׻� ���������� ������.
���� ������ ĭ�� ���ڿ� �����¿�� ������ ��� ĭ�� ������ ����.

�ֻ����� ���� ��,
1. ����� �´��� �ֻ����� ���� �ش� ĭ�� ���ں��� 'ũ��' ���� �������(�ʱ⿡ ������)���� 90�� �ð�������� ȸ��
2. ������, 90�� �ݽð� ȸ��
3. ������ ������� ����
!! ��������� ȸ���ϴ� ������ �ֻ����� ȸ���ϴ� ���� �ƴ�

!! ��������� �������� ����� �Ǹ� ���ݴ�� �ٲ�
*/

int sum = 7;
int board[21][21] = { {0,} };
int dirs[5][2] = { {-1, 0}, {0,1}, {1,0},{0,-1} }; // �� �� �� ��
int dir = 1;
int n, m;

int score = 0;

// ����, �� �� �� ��
int face = 1;
int sides[4] = { 5, 3, 2, 4 };
pair<int, int> pos = { 1, 1 };

bool isOut(int nr, int nc) {
    return nr > n || nc > n || nr < 1 || nc < 1;
}

void clockRotate() {
    dir = (dir + 1) % 4;
}

void counterClockRotate() {
    dir--;
    if (dir < 0) dir = 3;
}

void bfs() {
    int num = board[pos.R][pos.C];
    int count = 1;

    bool vis[21][21] = { 0, };
    vis[pos.R][pos.C] = 1;

    queue<pair<int, int>> que;
    que.push(pos);

    while (!que.empty()) {
        auto now = que.front();
        que.pop();

        for (auto& dir : dirs) {
            int nr = now.R + dir[0];
            int nc = now.C + dir[1];

            if (isOut(nr, nc) || vis[nr][nc] || board[nr][nc] != num) continue;

            count++;
            vis[nr][nc] = 1;
            que.push({ nr, nc });
        }
    }

    // ���� �߰�
    score += count * num;
}

// ������ �ظ��� ��ȯ
int roll() {
    int nr = pos.R + dirs[dir][0];
    int nc = pos.C + dirs[dir][1];

    if (isOut(nr, nc)) dir = (dir + 2) % 4; // ���ݴ�� ��ȯ
    // ���� ���� �ٽ� ���
    nr = pos.R + dirs[dir][0];
    nc = pos.C + dirs[dir][1];

    int bottom = sides[dir];

    int back = sum - face;
    int front = face;
    int newFace = sides[(dir + 2) % 4];

    face = newFace;
    sides[dir] = front;
    sides[(dir + 2) % 4] = back;

    pos = { nr, nc };

    return bottom;
}

void rotateDir() {
    int boardNum = board[pos.R][pos.C];
    int bottomNum = sum - face;

    if (bottomNum > boardNum) { // �ð����
        clockRotate();
    }
    else if (bottomNum < boardNum) { // �ݽð�
        counterClockRotate();
    }
}

int solve() {
    // ���⿡ �ڵ带 �ۼ����ּ���.
    cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> board[i][j];
        }
    }

    while (m-- > 0) {
        // 1. ������
        roll();
        // 2. �����߰�
        bfs();
        // 3. ȸ��
        rotateDir();
        // 4. �ٽ� ������
        roll();
    }

    cout << score << "\n";

    return 0;
}
