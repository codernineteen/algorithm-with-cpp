#include <algorithm>
#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <queue>
#include <cmath>

using namespace std;
#define R first
#define C second

/*
4x4 격자, m개의 몬스터, 1개의 팩맨
각 몬스터는 상하좌우 대각선 - 총 8개의 방향 중 하나를 가진다.

[개별 턴의 행동]
1. 몬스터 복제
    '현재 위치'에서 같은 방향을 가진 몬스터 복제 시도.
    복제 몬스터는 바로 부화되지 않음.

2. 몬스터 이동 (살아있는 몬스터끼리는 겹칠 수 있음.)
    자신이 가진 방향대로 한 칸 이동.
    몬스터 '시체'가 있거나, 팩맨이 있는 경우, 격자를 벗어나는 방향인 경우.
        -> 반시계 방향으로 45도 회전 (왼쪽으로 45도)
        -> 빈 공간이 보일 때까지 회전함
        -> !!!8방향을 다 돌았는데 움직일 수 없으면 멈춰있는다.!!!

3. 팩맨 이동(64개 완탐)
    총 3칸 이동. -> 각 이동마다 상하좌우의 선택지를 가진다.
    64가지의 경우의 수 중 몬스터를 가장 많이 먹을 수 있는 방향대로 이동.
        - 그러한 방향이 여러개면 상좌하우의 우선순위로 움직임.
    이동하는 과정에서 격자 바깥을 나가는 경우는 고려하지 않는다.
    먹은 몬스터는 그 칸에 시체로 남는다.
    알은 먹지 않는다.
    원래 있던 칸의 몬스터는 먹지 않음 (알 부화로 인해 가능함.)

4. 몬스터 시체 소멸
    시체 소멸은 두 턴이 걸림.

5. 몬스터 복제 완성
    이 단계에서 알이 부화.

몬스터와 팩맨의 초기 위치는 같을 수도 있다.

출력 : 모든 턴이 끝나고 살아 남은 몬스터의 마리 수를 출력.
*/

struct Monster {
    int row;
    int col;
    int dir;
    bool died;
};

int m, t;
int mdirs[8][2] = { {-1,0}, {-1,-1}, {0,-1},{1,-1},{1,0},{1,1},{0,1},{-1,1} };
int pdirs[4][2] = { {-1,0}, {0,-1}, {1,0}, {0,1} };

int mons[26][5][5][8] = { {{{0,}}} };
int dead[5][5][3] = { {{0,} } };
queue<Monster> waitMons;

int pr, pc;


bool canGo(int nr, int nc, int t) {
    if (nr > 4 || nc > 4 || nr < 1 || nc < 1) return false;
    if (dead[nr][nc][0] > 0 || dead[nr][nc][1] > 0) return false;
    if (nr == pr && nc == pc) return false;
    if (dead[nr][nc][t] > 0) return false;

    return true;
}

bool isOut(int nr, int nc) {
	if (nr > 4 || nc > 4 || nr < 1 || nc < 1) return true;
	return false;
}

void clone(int r, int c, int dir, int turn) {
    mons[turn][r][c][dir] += mons[turn-1][r][c][dir]; // 다음 턴에 부화
}

void moveMon(int r, int c, int dir, int turn) {
    // 반시계 방향 회전
    int nr = r + mdirs[dir][0];
    int nc = c + mdirs[dir][1];
    int orig = dir;

    if (canGo(nr, nc, turn)) {
        // 내가 가진 방향으로 갈 수 있는 경우.
        mons[turn][nr][nc][dir] += mons[turn-1][r][c][dir]; // 다음 자리 몬스터 증가
    }
    else {
        dir = (dir + 1) % 8; // 일단 회전

        while (orig != dir) { // 한바퀴 돌았으면 stop
            nr = r + mdirs[dir][0];
            nc = c + mdirs[dir][1];

            if (canGo(nr, nc, turn)) {
                mons[turn][nr][nc][dir] += mons[turn-1][r][c][orig]; // 다음 자리 몬스터 증가
                break; // 탈출
            }

            dir = (dir + 1) % 8; // 다음 방향 미리 회전
        }
    }
}

int getKill(int dir1, int dir2, int dir3, int turn) {
    int dirs[3] = {dir1, dir2, dir3};
    vector<vector<int>> visited(5, vector<int>(5, 0));

    int r = pr;
    int c = pc;
    int kill = 0;

    for (auto& dir : dirs) {
        int nr = r + pdirs[dir][0];
        int nc = c + pdirs[dir][1];
        if (isOut(nr, nc)) return -1;

        if (!visited[nr][nc]) {
            for(int i = 0; i < 8; i++) {
				kill += mons[turn][nr][nc][i];
			}

            visited[nr][nc] = 1;
        }

        r = nr;
        c = nc;
    }

    return kill;
}

void movePac(int turn) {
    int maxCnt = -1;
    int bestDir[3] = { 0,0,0 };

    for (int dir1 = 0; dir1 < 4; dir1++) {
        for (int dir2 = 0; dir2 < 4; dir2++) {
            for (int dir3 = 0; dir3 < 4; dir3++) {
                int curKill = getKill(dir1, dir2, dir3, turn);
                if (curKill > maxCnt) {
                    maxCnt= curKill;
                    bestDir[0] = dir1;
                    bestDir[1] = dir2;
                    bestDir[2] = dir3;
                }
            }
        }
    }


    // 저장한 최대 경로로 이동하며 업데이트
    for (auto& dir : bestDir) {
        int nr = pr + pdirs[dir][0];
        int nc = pc + pdirs[dir][1];

        for (int i = 0; i < 8; i++) {
            dead[nr][nc][2] += mons[turn][nr][nc][i];
            mons[turn][nr][nc][i] = 0;
        }
        pr = nr;
        pc = nc;
    }
}

void DecayM() {
    // decay를 진행합니다. 턴을 하나씩 깎아주면 됩니다.
    for (int i = 1; i <= 4; i++)
        for (int j = 1; j <= 4; j++) {
            for (int k = 0; k < 2; k++)
                dead[i][j][k] = dead[i][j][k + 1];
            dead[i][j][2] = 0;
        }
}

int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> m >> t;
    cin >> pr >> pc;
    

    for (int i = 1; i <= m; i++) {
        int r, c, dir;
        cin >> r >> c >> dir;
        mons[0][r][c][dir - 1]++;
    }



    int turn = 1;
    while (turn <= t) {

        for (int i = 1; i <= 4; i++) {
            for (int j = 1; j <= 4; j++) {
                for (int dir = 0; dir < 8; dir++) {
                    if (mons[turn][i][j][dir] > 0) {
                        moveMon(i, j, dir, turn);
                    }
                }
            }
        }

        // 3.팩맨 이동
        movePac(turn);


        //clone
        for (int i = 1; i <= 4; i++) {
			for (int j = 1; j <= 4; j++) {
				for (int dir = 0; dir < 8; dir++) {
                    clone(i, j, dir, turn);
				}
			}
		}

        turn++;
    }

    int remain = 0;
    for(int i = 1; i <= 4; i++) {
		for(int j = 1; j <= 4; j++) {
			for(int k = 0; k < 8; k++) {
                // 기존
				if(mons[turn-1][i][j][k] > 0) {
					remain += mons[turn-1][i][j][k];
				}
			}
		}
	}

    cout << remain << endl;

    return 0;
}
