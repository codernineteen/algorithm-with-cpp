#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <array>

#define R first
#define C second

using namespace std;

/*
L x L ü����, ��� (1,1)
ĭ ���� : ��ĭ | ���� | �� (ü���� �۵� ������ ����)
��� : (r,c) / ���� x �ʺ��� ���簢�� ���� / k ü��

1. ��� �̵� (�����¿�)
- �̵��Ϸ��� ��ġ�� �ٸ� ��簡 �ִٸ�, �� ��絵 �Բ� ���������� �и�.
- ���� ���� ���� ��簡 �̵��Ϸ��� ���⿡ ���� ������ ��� ���� �̵� �Ұ�
- �̹� ����� ���� ��ŵ

2. ��� �����
- �ٸ� ��縦 ��ġ�� ���� ���ظ� ����
    ���� : ��簡 �̵��� ��ġ�� '�ڽ��� ���� ���� �����ִ� ���� ����ŭ ����'
- ���ظ� ������ ü���� ���̰�, '���� ü�� �̻��� �����'�� �޴� �ٸ� ���
- !! ����� ���� ���� �̵��ص� ���ظ� ���� ����.
- ��� ����� �̵��� ���� �� ���ظ� �Դ´�.
*/

// 0 - ��ĭ, 1 - ����, 2 - ��
int board[41][41] = { {0,} };
int dirs[4][2] = { {-1,0}, {0,1}, {1,0}, {0, -1} };
int L, N, Q; // N - ���, Q - ���

pair<int, int> pos[31];
int health[31] = { 0, };
int width[31] = { 0, };
int height[31] = { 0, };
int orig[31] = { 0, };

bool isOut(int nr, int nc) {
    if (nr > L || nc > L || nr < 1 || nc < 1 || board[nr][nc] == 2) return true;
    return false;
}

vector<pair<int, int>> getSearchRange(int idx, int dir) {
    int r = pos[idx].R;
    int c = pos[idx].C;
    int w = width[idx];
    int h = height[idx];

    vector<pair<int, int>> search;

    if (dir == 0) { // ��
        r -= 1;
        for (int i = c; i <= c + w - 1; i++) {
            search.push_back({ r, i });
        }
    }
    else if (dir == 1) { // ��
        c += w;
        for (int i = r; i <= r + h - 1; i++) {
            search.push_back({ i, c });
        }
    }
    else if (dir == 2) { // ��
        r += h;
        for (int i = c; i <= c + w - 1; i++) {
            search.push_back({ r, i });
        }
    }
    else { // ��
        c -= 1;
        for (int i = r; i <= r + h - 1; i++) {
            search.push_back({ i, c });
        }
    }

    return search;
}

vector<int> move(int idx, int dir) {
    bool needMove[31] = { 0, };
    needMove[idx] = true;

    vector<int> hitted;
    queue<int> knights;
    knights.push(idx); // ���� ����~

    while (!knights.empty())
    {
        auto ind = knights.front();
        knights.pop();

        vector<pair<int, int>> search = getSearchRange(ind, dir);

        for (auto& next : search) {
            int nr = next.R;
            int nc = next.C;

            if (isOut(nr, nc)) return hitted;

            for (int i = 1; i <= N; i++) { // ���� ������ ��� ��� �� ����ִ� �ֵ�
                if (i != ind && health[i] > 0) {
                    int candR = pos[i].R;
                    int candC = pos[i].C;
                    int candW = width[i];
                    int candH = height[i];

                    if (candR <= nr && nr <= candR + candH - 1 && candC <= nc && nc <= candC + candW - 1) {
                        // ��ħ
                        needMove[i] = true;
                        knights.push(i);
                    }

                }
            }
        }
    }

    // �������� �ʿ��� �ֵ� ������Ʈ
    for (int i = 1; i <= N; i++) {
        if (needMove[i]) {
            pos[i].R += dirs[dir][0];
            pos[i].C += dirs[dir][1];
            hitted.push_back(i);
        }
    }

    return hitted;
}

void getDamage(int attacker, vector<int>& hitted) {
    for (auto& ind : hitted) {
        if (ind != attacker && health[ind] > 0) { // ���� �ƴϰ�, ����ִ� �ֵ�
            int cr = pos[ind].R;
            int cc = pos[ind].C;

            for (int r = cr; r <= cr + height[ind] - 1; r++) {
                for (int c = cc; c <= cc + width[ind] - 1; c++) {
                    if (board[r][c] == 1) { // �����̸�
                        health[ind] = max(0, health[ind] - 1);
                    }
                }
            }
        }
    }
}

int solve() {
    cin >> L >> N >> Q;

    for (int i = 1; i <= L; i++) {
        for (int j = 1; j <= L; j++) {
            cin >> board[i][j];
        }
    }

    for (int i = 1; i <= N; i++) {
        cin >> pos[i].R >> pos[i].C >> height[i] >> width[i] >> health[i];
        orig[i] = health[i]; // ���� ��� ������ ���� ü�� ���
    }

    while (Q-- > 0) {
        // �̹� ����� ����� ��ȣ�� �־��� �� �ִ�.
        int idx, dir;
        cin >> idx >> dir;

        if (health[idx] == 0) continue;

        auto hitted = move(idx, dir);
        if (hitted.empty()) continue; // �������� �ʾ���.
        getDamage(idx, hitted);
    }

    // �� ���� ����� �� ���
    int total = 0;
    for (int i = 1; i <= N; i++) {
        if (health[i] == 0) continue;
        total += orig[i] - health[i];
    }

    cout << total;

    return 0;
}
