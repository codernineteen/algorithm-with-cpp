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
N x N 격자,
미로 칸
    1. 이동 가능
    2. 벽 , 1 ~ 9 내구도 -> 회전하면 내구도가 1깎임 ->0이 되면 빈칸
    3. 출구, -> 도착하면 즉시 탈출

참가자
    두 위치의 최단 거리에 따라 이동.
    모든 참가자는 상하좌우 중 가능한 칸으로 동시에 움직인다.
    이동 칸은 무조건 출구랑 가까워야 함. -> 2개 이상이라면 상하로 움직이는 것이 우선
    움직일 수 없다면 움직이지 않음
    한 칸에 여러 참가자가 있을 수 있다.

미로 회전(1x1 부터 nxn까지 완탐 , 찾으면 break)
    한 명 이상의 참가자와 출구를 포함한 가장 작은 정사각형
    선택된 사각형은 시계방향으로 90도 회전
    사각형 안에 포함된 벽은 내구도가 깎임.
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
    int dist = getDist(cur.R, cur.C, exitR, exitC); // 현재 머물러 있던 칸 과의 거리로 시작.

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

    if (next.R == 0 && next.C == 0) return; // 움직일 수 없음

    walks[pid]++;
    pos[pid] = next;
    // 탈출임?
    if (pos[pid].R == exitR && pos[pid].C == exitC) isExit[pid] = true;
}

vector<int> findSquareTopLeft() {
    // 모든 가능한 정사각형 길이
    int len;
    int nr;
    int nc;

    for (int side = 1; side <= N; side++) {
        // 현재 변 길이에 대해 탐색 가능한 좌표
        for (int r = 1; r <= N - side + 1; r++) {
            for (int c = 1; c <= N - side + 1; c++) {
                bool people = false;
                bool exit = false;

                // 현재 좌표에서 정사각형 요소 탐색
                for (int i = r; i < r + side; i++) {
                    for (int j = c; j < c + side; j++) {
                        // 출구좌표랑 같은지
                        if (i == exitR && j == exitC) exit = true;

                        for (int pid = 1; pid <= M; pid++) {
                            // 탈출하지 않은 사람을 포함하는지
                            if (!isExit[pid] && pos[pid].R == i && pos[pid].C == j) people = true;
                        }
                    }
                }

                // 출구랑 사람을 포함하는 가장 작은 정사각형
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
                // 좌표가 탈출하지 않음 사람과 같은지
                if (!isExit[pid] && pos[pid].R == row && pos[pid].C == col) people = pid;
            }

            // 행, 열 원점 기준으로 변환
            cr -= sr;
            cc -= sc;
            // 회전된 원점 기준 좌표 
            int nr = cc;
            int nc = len - cr - 1;
            square[nr][nc] = value; // 원소 삽입

            // 회전된 sr, sc기준 좌표
            if (people > 0) {
                rotatedPeople.push_back({ people, nr + sr, nc + sc });
            }
            if (exit) {
                rotatedExit = { nr + sr, nc + sc };
            }
        }
    }

    // 회전에 따른 결과 반영
    for (int row = sr, i = 0; row < sr + len; row++, i++) {
        for (int col = sc, j = 0; col < sc + len; col++, j++) {
            board[row][col] = square[i][j];
            if (board[row][col]) board[row][col]--;
        }
    }

    // 출구 회전, 사람 회전
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
//    // 회전에 따른 결과 반영
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
        // 모든 참가자 동시에 이동.
        for (int pid = 1; pid <= M; pid++) {
            if (isExit[pid]) continue; // 이미 탈출했으면 스킵
            move(pid);
        }

        bool isEnd = true;
        for (int pid = 1; pid <= M; pid++) {
            if (!isExit[pid]) isEnd = false;
        }
        if (isEnd) {
            break;
        }

        // 한 명 이상의 참가자와 출구를 포함한 가장 작은 정사각형
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
