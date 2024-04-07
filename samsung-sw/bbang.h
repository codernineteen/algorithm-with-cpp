#include "types.h"
// ���� n x n
// m���� ���� ���ϴ� ���, 1�� ��� 1�� ��� -> M���� �ݺ�
// ������ ���̽� ķ������ ����ϰ�, ��� ������ ���� �ٱ����� ���
// ���ڰ� ��ǥ�ϴ� �������� '���' �ٸ�

// �ൿ ��Ģ 1 -> 2 -> 3
/*
�� �ൿ�� 1�а� ��� �̷������. ���� ���� ����� ���� ���� 3��-���̽� ķ�� ã�� ���� ����.
1. ���� ���� ������ ���ؼ� 1ĭ(�켱���� : ���¿���)
2. �������� �����ϸ� �����. ���� �������� �����ϸ� ���� ������� �� ĭ�� ������ �� ����.
3. ���� �ð� t(<=m) �� ��, t�� ����� �������� ���� ������ �ִ� ���̽� ķ���� ����.
   ���� : �ִܰŸ�(����ư �Ÿ�) -> ���� �� ���� ���� -> ���� �� ���� ����
   ���̽� ķ������ �̵����� �ð� �ҿ� ����
   �ٸ� ����� t�������� ���̽� ķ���� '������' ������ �� ����
*/
using namespace std;

#define R first
#define C second

//int dirs[4][2] = { {-1,0}, {0,-1}, {0, 1}, {1,0} };
int dirs[4][2] = { {1,0},{0,1},{0,-1},{-1,0} };

int n, m;
int board[16][16] = { {0,} };
int people[31][2] = { {0,} };
int dests[31][2] = { {0,} };
int remain;

int getDist(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

pair<int, int> bfs(int pInd, bool isBase) {
    int tr = dests[pInd][0];
    int tc = dests[pInd][1];

    queue<vector<int>> que;
    que.push({ tr, tc, 0 });
    
    vector<vector<int>> cands;
    vector<vector<int>> vis(n+1, vector<int>(n+1, 0));
    vis[tr][tc] = 1;

    while (!que.empty()) {
        vector<int> cur = que.front();
        que.pop();

        if (isBase && board[cur[0]][cur[1]] == 1) {
            cands.push_back({ cur[0], cur[1], cur[2] });
            continue;
        }

        for (auto& dir : dirs) {
            int nr = cur[0] + dir[0];
            int nc = cur[1] + dir[1];

            if (nr < 1 || nr > n || nc < 1 || nc > n || vis[nr][nc] == 1) continue;
            
            vis[nr][nc] = 1;
            if (isBase) {
                if (board[nr][nc] == -1) continue;
                que.push({ nr, nc, cur[2] + 1 });
            }
            else {
                if (nr == people[pInd][0] && nc == people[pInd][1]) {
                    cands.push_back({ cur[0], cur[1], cur[2] });
                }
                else {
                    if (board[nr][nc] == -1) continue;
                    que.push({ nr, nc, cur[2] + 1 });
                }
            }
        }
    }

    vector<int> next = cands.front();
    if (isBase) {
        for (int i = 0; i < cands.size(); i++) {
            if (cands[i][2] < next[2]) {
                next = cands[i];
            }
            else if (cands[i][2] == next[2]) {
                if (cands[i][0] < next[0]) {
                    next = cands[i];
                }
                else if (cands[i][0] == next[0]) {
                    if (cands[i][1] < next[1]) {
                        next = cands[i];
                    }
                }
            }
        }
    }


    return { next[0], next[1] };
}

bool checkPosition(int pInd) {
    // �����ϸ� people�� �ش� ��� (0,0) ������Ʈ / ���� ���� -1�� ������Ʈ
    int curR = people[pInd][0];
    int curC = people[pInd][1];
    int tr = dests[pInd][0];
    int tc = dests[pInd][1];
    if (curR == tr && curC == tc) { // ����
        people[pInd][0] = 0;
        people[pInd][1] = 0;
        remain--;
        return true;
    }

    return false;
}

int solve() {
    cin >> n >> m;
    remain = m;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> board[i][j];
        }
    }

    for (int i = 1; i <= m; i++) {
        cin >> dests[i][0] >> dests[i][1];
    }

    int t = 1;
    while (remain > 0) {
        vector<int> ends;

        for (int i = 1; i <= m; i++) {
            if (people[i][0] == 0 && people[i][1] == 0) continue;

            pair<int, int> next = bfs(i, false);
            people[i][0] = next.R;
            people[i][1] = next.C;

            bool ret = checkPosition(i);
            if (ret) ends.push_back(i);
        }
        for (auto& e : ends) {
            board[dests[e][0]][dests[e][1]] = -1; // ���ɵ� ������
        }
        if (t <= m) {
           pair<int, int> basePos = bfs(t, true);
           people[t][0] = basePos.R;
           people[t][1] = basePos.C;
           board[basePos.R][basePos.C] = -1;
        }

        t++;
    }

    cout << t - 1 << "\n";

    return 0;
}
