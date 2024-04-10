#include "types.h"

#define row first
#define col second

using namespace std;

/*
게임판 : n x n 격자 , m 개의 턴
매턴마다 루돌프와 살아남은 산타들이 한 번씩 움직임.(루돌프 -> 1번 산타 -> ... -> p 번 산타)
거리 : (r1-r2)^2 + (c1-c2)^2

1. 루돌프
    - 가장 가까운 거리에 있는 '탈락하지 않은' 산타 (거리는 위의 공식)
    - 두 명 이상이면 r 좌표가 큰 산타 -> c 좌표가 큰 산타
    - 루돌프 8방향 가능. 해당 산타와 가장 가까워지는 방향으로 한 칸 돌진(대각선도 한칸으로 침.) - bfs

2. 산타
    - 루돌프에게 가장 가까워지는 방향으로 1칸 이동.
    - 다른 산타가 있는 칸이나 게임밖으로 불가능 -> 움직일 수 있는 칸이 없으면 정지
    - 상우하좌 우선순위에 따라 움직임.

3. 충돌
    - 산타와 루돌프가 같은 칸에 있을 때
        - 루돌프가 박치기 한 경우 , 루돌프가 이동해온 방향으로 c칸만큼 밀려남. (산타 c점 획득)
        - 산타가 부딪힌 경우, 자신이 이동해온 반대로 D칸 밀려남 (루돌프 d점 획득)
        - 착지 지점이 바깥이면 탈락.
        - 밀려난 칸에 다른 산타있으면 상호작용 시작

4. 상호작용
    - 밀려난 칸에 다른 산타가 있으면 연쇄적으로 1칸씩 같은 방향으로 밀림.

5. 기절
    - k번째 턴에 기절이면, k+2번째부터 정상
    - 기절 도중에도 밀려날 수 있음.
    - 기절한 산타가 돌진 대상이 될 수 있음
6. 종료
    - m턴 이후 즉시 종료
    - 산타가 모두 탈락하면 즉시 종료
    - 탈락하지 않은 산타는 1점을 얻음.
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
        if (outs[idx] == true) continue; //이미 탈락한 산타는 무시

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

    // 일단 날라가
    board[curR][curC] = 0;

    // 1. 경기장 밖
    if (isOut(nr, nc)) {
        outs[sid] = true; // 탈락 처리
        sPos[sid].row = 0;
        sPos[sid].col = 0;
    }
    else { // 2. 경기장 안
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

            // 상호작용에 포함된 맨 뒤 산타부터 시작.
            for (int i = outCand.size() - 1; i >= 0; i--) {
                int s = outCand[i]; // 산타 식별자
                int pushR = sPos[s].row;
                int pushC = sPos[s].col;

                if (isOut(pushR + dir[0], pushC + dir[1])) {
                    outs[s] = true; // 탈락 처리
                    board[pushR][pushC] = 0; // 보드 마킹 초기화
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
        // 충돌 처리
        int sid = board[next.row][next.col];
        vector<int> dir{ next.row - rPos.row, next.col - rPos.col };
        scores[sid] += C;
        crash(sid, C, dir, true);
    }

    // 충돌 처리 후 혹은 바로 이동.
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

    if (board[next.row][next.col] > 0) return; // 다른 산타 있음

    if (board[next.row][next.col] == -1) {
        vector<int> dir{ sPos[sid].row - next.row, sPos[sid].col - next.col };
        scores[sid] += D;

        board[next.row][next.col] = sid;
        board[sPos[sid].row][sPos[sid].col] = 0;
        sPos[sid].row = next.row;
        sPos[sid].col = next.col;

        crash(sid, D, dir, false);

        board[next.row][next.col] = -1; // 마지막엔 꼭 다시 루돌프로 마킹
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

        if (stuns[i] > 0) stuns[i]--; // 기절이면 기절 스택 감소
        else trace(i); // 추적
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

        // 모두 탈락이면 즉시 종료.
        bool isFinish = true;
        for (int i = 1; i <= P; i++) {
            if (!outs[i]) {
                scores[i]++; // 1점 추가
                isFinish = false;
            }
        }
        if (isFinish) break;
    }

    for (int i = 1; i <= P; i++) cout << scores[i] << " ";

    return 0;
}
