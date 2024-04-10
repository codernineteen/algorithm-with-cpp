#include "types.h"

#define row first
#define col second

using namespace std;

/*
������ : n x n ���� , m ���� ��
���ϸ��� �絹���� ��Ƴ��� ��Ÿ���� �� ���� ������.(�絹�� -> 1�� ��Ÿ -> ... -> p �� ��Ÿ)
�Ÿ� : (r1-r2)^2 + (c1-c2)^2

1. �絹��
    - ���� ����� �Ÿ��� �ִ� 'Ż������ ����' ��Ÿ (�Ÿ��� ���� ����)
    - �� �� �̻��̸� r ��ǥ�� ū ��Ÿ -> c ��ǥ�� ū ��Ÿ
    - �絹�� 8���� ����. �ش� ��Ÿ�� ���� ��������� �������� �� ĭ ����(�밢���� ��ĭ���� ħ.) - bfs

2. ��Ÿ
    - �絹������ ���� ��������� �������� 1ĭ �̵�.
    - �ٸ� ��Ÿ�� �ִ� ĭ�̳� ���ӹ����� �Ұ��� -> ������ �� �ִ� ĭ�� ������ ����
    - ������� �켱������ ���� ������.

3. �浹
    - ��Ÿ�� �絹���� ���� ĭ�� ���� ��
        - �絹���� ��ġ�� �� ��� , �絹���� �̵��ؿ� �������� cĭ��ŭ �з���. (��Ÿ c�� ȹ��)
        - ��Ÿ�� �ε��� ���, �ڽ��� �̵��ؿ� �ݴ�� Dĭ �з��� (�絹�� d�� ȹ��)
        - ���� ������ �ٱ��̸� Ż��.
        - �з��� ĭ�� �ٸ� ��Ÿ������ ��ȣ�ۿ� ����

4. ��ȣ�ۿ�
    - �з��� ĭ�� �ٸ� ��Ÿ�� ������ ���������� 1ĭ�� ���� �������� �и�.

5. ����
    - k��° �Ͽ� �����̸�, k+2��°���� ����
    - ���� ���߿��� �з��� �� ����.
    - ������ ��Ÿ�� ���� ����� �� �� ����
6. ����
    - m�� ���� ��� ����
    - ��Ÿ�� ��� Ż���ϸ� ��� ����
    - Ż������ ���� ��Ÿ�� 1���� ����.
*/



int N, M, P, C, D;
int board[51][51] = { {0,} };

vector<vector<int>> rdirs{ {-1,0}, {0,1}, {1,0}, {0,-1}, {1,1}, {1,-1}, {-1,1}, {-1,-1} };
vector<vector<int>> sdirs{ {-1,0}, {0,1},{1,0},{0,-1} };

vector<pair<int, int>> sPos(31, pair<int, int>());
pair<int, int> rPos;

int scores[31] = { 0, };
int stuns[31] = { 0, };
bool outs[31] = { 0, };

bool isOut(int nr, int nc) {
    return nr > N || nc > N || nr < 1 || nc < 1;
}

int getDist(int r1, int c1, int r2, int c2) {
    return (r1 - r2) * (r1 - r2) + (c1 - c2) * (c1 - c2);
}

int findSanta() {
    int sid = 1;
    int dist = INT_MAX;

    for (int idx = 1; idx <= P; idx++) {
        if (outs[idx] == true) continue; //�̹� Ż���� ��Ÿ�� ����

        auto& santa = sPos[idx];
        int cDist = getDist(rPos.row, rPos.col, santa.row, santa.col);
        if (cDist < dist) {
            sid = idx;
            dist = cDist;
        }
        else if (cDist == dist) {
            if (santa.row > sPos[sid].row) {
                sid = idx;
                dist = cDist;
            }
            else if (santa.row == sPos[sid].row) {
                if (santa.col > sPos[sid].col) {
                    sid = idx;
                    dist = cDist;
                }
            }
        }
    }

    return sid;
}

void crash(int sid, int power, vector<int>& dir, bool hitted) {
    int curR = sPos[sid].row;
    int curC = sPos[sid].col;
 
    int nr = curR + dir[0] * power;
	int nc = curC + dir[1] * power;

    // �ϴ� ����
    board[curR][curC] = 0;

    // 1. ����� ��
    if (isOut(nr, nc)) {
        outs[sid] = true; // Ż�� ó��
        sPos[sid].row = 0;
        sPos[sid].col = 0;
    }
    else { // 2. ����� ��
        if (board[nr][nc] != 0) {
            vector<int> outCand;
            int cr = nr;
            int cc = nc;
            while (!isOut(cr, cc)) {
                if (board[cr][cc] != 0) {
                    outCand.push_back(board[cr][cc]);
                    cr += dir[0];
                    cc += dir[1];
                    continue;
                }
                break;
            }

            // ��ȣ�ۿ뿡 ���Ե� �� �� ��Ÿ���� ����.
            for (int i = outCand.size() - 1; i >= 0; i--) {
                int s = outCand[i]; // ��Ÿ �ĺ���
                int pushR = sPos[s].row;
                int pushC = sPos[s].col;

                if (isOut(pushR + dir[0], pushC + dir[1])) {
                    outs[s] = true; // Ż�� ó��
                    board[pushR][pushC] = 0; // ���� ��ŷ �ʱ�ȭ
                    sPos[s].row = 0;
                    sPos[s].col = 0;
                }
                else {
                    swap(board[pushR + dir[0]][pushC + dir[1]], board[pushR][pushC]);
                    sPos[s].row = pushR + dir[0];
                    sPos[s].col = pushC + dir[1];
                }
            }
        }

        if (hitted) {
            stuns[sid] = 2;
        }
        else {
            stuns[sid] = 1;
        }
        board[nr][nc] = sid;
        sPos[sid].row = nr;
        sPos[sid].col = nc;
    }
}

void protest() {
    int tid = findSanta();

    pair<int, int> next = { rPos.row, rPos.col };
    int dist = INT_MAX;

    for(auto& dir : rdirs) {
		int nr = rPos.row + dir[0];
		int nc = rPos.col + dir[1];
        int cDist = getDist(nr, nc, sPos[tid].row, sPos[tid].col);
		if (isOut(nr, nc)) continue;
		if (cDist < dist) {
			next = { nr, nc };
            dist = cDist;
		}
	}


    if (board[next.row][next.col] > 0) {
        // �浹 ó��
        int sid = board[next.row][next.col];
        vector<int> dir{ next.row - rPos.row, next.col - rPos.col };
        scores[sid] += C;
        crash(sid, C, dir, true);
    }

    // �浹 ó�� �� Ȥ�� �ٷ� �̵�.
    swap(board[next.row][next.col], board[rPos.row][rPos.col]);
    rPos.row = next.row;
    rPos.col = next.col;

}

void trace(int sid) {
    pair<int, int> next = { sPos[sid].row, sPos[sid].col };
    int dist = getDist(next.row, next.col, rPos.row, rPos.col);

    for (auto& dir : sdirs) {
        int nr = sPos[sid].row + dir[0];
        int nc = sPos[sid].col + dir[1];

        if (isOut(nr, nc)) continue;
        if (board[nr][nc] > 0) continue;
        if (getDist(nr, nc, rPos.row, rPos.col) < dist) {
            next = { nr, nc };
            dist = getDist(nr, nc, rPos.row, rPos.col);
        }
    }

    if (board[next.row][next.col] > 0) return; // �ٸ� ��Ÿ ����

    if (board[next.row][next.col] == -1) {
        vector<int> dir{ sPos[sid].row - next.row, sPos[sid].col - next.col };
        scores[sid] += D;

        board[next.row][next.col] = sid;
        board[sPos[sid].row][sPos[sid].col] = 0;
        sPos[sid].row = next.row;
        sPos[sid].col = next.col;

        crash(sid, D, dir, false);

        board[next.row][next.col] = -1; // �������� �� �ٽ� �絹���� ��ŷ
    }
    else {
        swap(board[next.row][next.col], board[sPos[sid].row][sPos[sid].col]);
        sPos[sid].row = next.row;
        sPos[sid].col = next.col;
    }
}


void simulate() {
    protest();
    for (int i = 1; i <= P; i++) {
        if (outs[i]) continue;

        if (stuns[i] > 0) stuns[i]--; // �����̸� ���� ���� ����
        else trace(i); // ����
    }
}

int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> M >> P >> C >> D;

    cin >> rPos.row >> rPos.col;
    board[rPos.row][rPos.col] = -1;

    for (int i = 1; i <= P; i++) {
        int ind, r, c;
        cin >> ind >> r >> c;
        sPos[ind] = { r,c };
        board[r][c] = ind;
    }

    while (M-- > 0) {
        simulate();

        // ��� Ż���̸� ��� ����.
        bool isFinish = true;
        for (int i = 1; i <= P; i++) {
            if (!outs[i]) {
                scores[i]++; // 1�� �߰�
                isFinish = false;
            }
        }
        if (isFinish) break;
    }

    for (int i = 1; i <= P; i++) cout << scores[i] << " ";

    return 0;
}
