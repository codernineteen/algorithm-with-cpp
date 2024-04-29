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
4x4 ����, m���� ����, 1���� �Ѹ�
�� ���ʹ� �����¿� �밢�� - �� 8���� ���� �� �ϳ��� ������.

[���� ���� �ൿ]
1. ���� ����
    '���� ��ġ'���� ���� ������ ���� ���� ���� �õ�.
    ���� ���ʹ� �ٷ� ��ȭ���� ����.

2. ���� �̵� (����ִ� ���ͳ����� ��ĥ �� ����.)
    �ڽ��� ���� ������ �� ĭ �̵�.
    ���� '��ü'�� �ְų�, �Ѹ��� �ִ� ���, ���ڸ� ����� ������ ���.
        -> �ݽð� �������� 45�� ȸ�� (�������� 45��)
        -> �� ������ ���� ������ ȸ����
        -> !!!8������ �� ���Ҵµ� ������ �� ������ �����ִ´�.!!!

3. �Ѹ� �̵�(64�� ��Ž)
    �� 3ĭ �̵�. -> �� �̵����� �����¿��� �������� ������.
    64������ ����� �� �� ���͸� ���� ���� ���� �� �ִ� ������ �̵�.
        - �׷��� ������ �������� �����Ͽ��� �켱������ ������.
    �̵��ϴ� �������� ���� �ٱ��� ������ ���� ������� �ʴ´�.
    ���� ���ʹ� �� ĭ�� ��ü�� ���´�.
    ���� ���� �ʴ´�.
    ���� �ִ� ĭ�� ���ʹ� ���� ���� (�� ��ȭ�� ���� ������.)

4. ���� ��ü �Ҹ�
    ��ü �Ҹ��� �� ���� �ɸ�.

5. ���� ���� �ϼ�
    �� �ܰ迡�� ���� ��ȭ.

���Ϳ� �Ѹ��� �ʱ� ��ġ�� ���� ���� �ִ�.

��� : ��� ���� ������ ��� ���� ������ ���� ���� ���.
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
    mons[turn][r][c][dir] += mons[turn-1][r][c][dir]; // ���� �Ͽ� ��ȭ
}

void moveMon(int r, int c, int dir, int turn) {
    // �ݽð� ���� ȸ��
    int nr = r + mdirs[dir][0];
    int nc = c + mdirs[dir][1];
    int orig = dir;

    if (canGo(nr, nc, turn)) {
        // ���� ���� �������� �� �� �ִ� ���.
        mons[turn][nr][nc][dir] += mons[turn-1][r][c][dir]; // ���� �ڸ� ���� ����
    }
    else {
        dir = (dir + 1) % 8; // �ϴ� ȸ��

        while (orig != dir) { // �ѹ��� �������� stop
            nr = r + mdirs[dir][0];
            nc = c + mdirs[dir][1];

            if (canGo(nr, nc, turn)) {
                mons[turn][nr][nc][dir] += mons[turn-1][r][c][orig]; // ���� �ڸ� ���� ����
                break; // Ż��
            }

            dir = (dir + 1) % 8; // ���� ���� �̸� ȸ��
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


    // ������ �ִ� ��η� �̵��ϸ� ������Ʈ
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
    // decay�� �����մϴ�. ���� �ϳ��� ����ָ� �˴ϴ�.
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

        // 3.�Ѹ� �̵�
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
                // ����
				if(mons[turn-1][i][j][k] > 0) {
					remain += mons[turn-1][i][j][k];
				}
			}
		}
	}

    cout << remain << endl;

    return 0;
}
