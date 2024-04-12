#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <cmath>
#include <climits>

using namespace std;

#define R first
#define C second

/*
N x N ����,
�̷� ĭ
    1. �̵� ����
    2. �� , 1 ~ 9 ������ -> ȸ���ϸ� �������� 1���� ->0�� �Ǹ� ��ĭ
    3. �ⱸ, -> �����ϸ� ��� Ż��

������
    �� ��ġ�� �ִ� �Ÿ��� ���� �̵�.
    ��� �����ڴ� �����¿� �� ������ ĭ���� ���ÿ� �����δ�.
    �̵� ĭ�� ������ �ⱸ�� ������� ��. -> 2�� �̻��̶�� ���Ϸ� �����̴� ���� �켱
    ������ �� ���ٸ� �������� ����
    �� ĭ�� ���� �����ڰ� ���� �� �ִ�.

�̷� ȸ��(1x1 ���� nxn���� ��Ž , ã���� break)
    �� �� �̻��� �����ڿ� �ⱸ�� ������ ���� ���� ���簢��
    ���õ� �簢���� �ð�������� 90�� ȸ��
    �簢�� �ȿ� ���Ե� ���� �������� ����.
*/

int N, M, K;
int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0, 1} };
int board[11][11] = { {0,} };

pair<int, int> pos[11];
int walks[11] = { 0, };
bool isExit[11] = { false, };
int exitR;
int exitC;

bool canGo(int nr, int nc) {
    return nr <= N && nc <= N && nr >= 1 && nc >= 1 && (board[nr][nc] == 0);
}

int getDist(int r1, int c1, int r2, int c2) {
    return abs(r1 - r2) + abs(c1 - c2);
}

void move(int pid) {
    auto& cur = pos[pid];

    if (cur.R == exitR && cur.C == exitC) return;

    pair<int, int> next = { 0,0 };
    int dist = getDist(cur.R, cur.C, exitR, exitC); // ���� �ӹ��� �ִ� ĭ ���� �Ÿ��� ����.

    for (auto& dir : dirs) {
        int nr = cur.R + dir[0];
        int nc = cur.C + dir[1];

        if (!canGo(nr, nc)) continue;

        int curDist = getDist(exitR, exitC, nr, nc);
        if (curDist < dist) {
            dist = curDist;
            next = { nr, nc };
        }
    }

    if (next.R == 0 && next.C == 0) return; // ������ �� ����

    walks[pid]++;
    pos[pid] = next;
    // Ż����?
    if (pos[pid].R == exitR && pos[pid].C == exitC) isExit[pid] = true;
}

vector<int> findSquareTopLeft() {
    // ��� ������ ���簢�� ����
    int len;
    int nr;
    int nc;

    for (int side = 1; side <= N; side++) {
        // ���� �� ���̿� ���� Ž�� ������ ��ǥ
        for (int r = 1; r <= N - side + 1; r++) {
            for (int c = 1; c <= N - side + 1; c++) {
                bool people = false;
                bool exit = false;

                // ���� ��ǥ���� ���簢�� ��� Ž��
                for (int i = r; i < r + side; i++) {
                    for (int j = c; j < c + side; j++) {
                        // �ⱸ��ǥ�� ������
                        if (i == exitR && j == exitC) exit = true;

                        for (int pid = 1; pid <= M; pid++) {
                            // Ż������ ���� ����� �����ϴ���
                            if (!isExit[pid] && pos[pid].R == i && pos[pid].C == j) people = true;
                        }
                    }
                }

                // �ⱸ�� ����� �����ϴ� ���� ���� ���簢��
                if (people && exit) {
                    len = side;
                    nr = r;
                    nc = c;
                    return { nr, nc, len };
                }
            }
        }
    }
}

void rotateSquare(int sr, int sc, int len) {
    vector<vector<int>> square(len, vector<int>(len, 0));

    vector<vector<int>> rotatedPeople;
    pair<int, int> rotatedExit;

    for (int row = sr; row < sr + len; row++) {
        for (int col = sc; col < sc + len; col++) {
            int cr = row;
            int cc = col;

            int value = board[row][col];

            int people = 0;
            bool exit = false;

            if (row == exitR && col == exitC) exit = true;
            for (int pid = 1; pid <= M; pid++) {
                // ��ǥ�� Ż������ ���� ����� ������
                if (!isExit[pid] && pos[pid].R == row && pos[pid].C == col) people = pid;
            }

            // ��, �� ���� �������� ��ȯ
            cr -= sr;
            cc -= sc;
            // ȸ���� ���� ���� ��ǥ 
            int nr = cc;
            int nc = len - cr - 1;
            square[nr][nc] = value; // ���� ����

            // ȸ���� sr, sc���� ��ǥ
            if (people > 0) {
                rotatedPeople.push_back({ people, nr + sr, nc + sc });
            }
            if (exit) {
                rotatedExit = { nr + sr, nc + sc };
            }
        }
    }

    // ȸ���� ���� ��� �ݿ�
    for (int row = sr, i = 0; row < sr + len; row++, i++) {
        for (int col = sc, j = 0; col < sc + len; col++, j++) {
            board[row][col] = square[i][j];
            if (board[row][col]) board[row][col]--;
        }
    }

    // �ⱸ ȸ��, ��� ȸ��
    exitR = rotatedExit.R;
    exitC = rotatedExit.C;
    for (auto& person : rotatedPeople) {
        int id = person[0];
        pos[id] = { person[1], person[2] };
    }
}



//void rotateSquare(int sr, int sc, int len) {
//    int nextBoard[11][11] = { {0,} };
//
//    vector<vector<int>> rotatedPeople;
//    pair<int, int> rotatedExit;
//
//    // ȸ���� ���� ��� �ݿ�
//    for (int r = sr; r < sr + len; r++) {
//        for (int c = sc; c < sc + len; c++) {
//            if(board[r][c] > 0) board[r][c]--;
//        }
//    }
//
//    for (int row = sr; row < sr + len; row++) {
//        for (int col = sc; col < sc + len; col++) {
//            int value = board[row][col];
//            int cr = row - sr;
//            int cc = col - sc;
//
//            int nr = cc;
//            int nc = len - cr - 1;
//            nextBoard[nr + sr][nc + sc] = value;
//
//        }
//    }
//
//    for (int r = sr; r < sr + len; r++) {
//        for (int c = sc; c < sc + len; c++) {
//            board[r][c] = nextBoard[r][c];
//        }
//    }
//}

void rotateExitAndPeople(int sr, int sc, int len) {
    for (int i = 1; i <= M; i++) {
        if (isExit[i]) continue;
		auto& cur = pos[i];

        if (cur.R >= sr && cur.R < sr + len && cur.C >= sc && cur.C < sc + len) {
            int cr = cur.R - sr;
            int cc = cur.C - sc;

            int nr = cc;
            int nc = len - cr - 1;
            pos[i] = { nr + sr, nc + sc };
        }	
    }

    if(exitR >= sr && exitR < sr + len && exitC >= sc && exitC < sc + len) {
		int cr = exitR - sr;
		int cc = exitC - sc;

		int nr = cc;
		int nc = len - cr - 1;
		exitR = nr + sr;
		exitC = nc + sc;
	}
}


int solve() {
    cin >> N >> M >> K;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= N; j++) {
            cin >> board[i][j];
        }
    }

    for (int i = 1; i <= M; i++) {
        cin >> pos[i].R >> pos[i].C;
    }

    cin >> exitR >> exitC;

    while (K-- > 0) {
        // ��� ������ ���ÿ� �̵�.
        for (int pid = 1; pid <= M; pid++) {
            if (isExit[pid]) continue; // �̹� Ż�������� ��ŵ
            move(pid);
        }

        bool isEnd = true;
        for (int pid = 1; pid <= M; pid++) {
            if (!isExit[pid]) isEnd = false;
        }
        if (isEnd) {
            break;
        }

        // �� �� �̻��� �����ڿ� �ⱸ�� ������ ���� ���� ���簢��
        vector<int> square = findSquareTopLeft();
        rotateSquare(square[0], square[1], square[2]);
        rotateExitAndPeople(square[0], square[1], square[2]);
    }

    int res = 0;
    for (int i = 1; i <= M; i++) res += walks[i];
    cout << res << "\n";
    cout << exitR << " " << exitC << "\n";



    return 0;
}
