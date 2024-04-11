#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>

using namespace std;

#define R first
#define C second
/*
n x n 격자, 마주보는 숫자의 합은 7, 면의 넓이가 1 x 1 인 정육면체 주사위
1,1 부터 시작 , 처음에는 항상 오른쪽으로 움직임.
내가 도착한 칸의 숫자와 상하좌우로 인접한 모든 칸의 점수를 얻음.

주사위를 굴린 후,
1. 보드와 맞닿은 주사위의 면이 해당 칸의 숫자보다 '크면' 현재 진행방향(초기에 오른쪽)에서 90도 시계방향으로 회전
2. 작으면, 90도 반시계 회전
3. 같으면 진행방향 유지
!! 진행방향이 회전하는 것이지 주사위가 회전하는 것이 아님

!! 진행방향이 격자판을 벗어나게 되면 정반대로 바뀜
*/

int sum = 7;
int board[21][21] = { {0,} };
int dirs[5][2] = { {-1, 0}, {0,1}, {1,0},{0,-1} }; // 상 우 하 좌
int dir = 1;
int n, m;

int score = 0;

// 윗면, 상 우 하 좌
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

    // 점수 추가
    score += count * num;
}

// 굴리고 밑면을 반환
int roll() {
    int nr = pos.R + dirs[dir][0];
    int nc = pos.C + dirs[dir][1];

    if (isOut(nr, nc)) dir = (dir + 2) % 4; // 정반대로 전환
    // 다음 방향 다시 계산
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

    if (bottomNum > boardNum) { // 시계방향
        clockRotate();
    }
    else if (bottomNum < boardNum) { // 반시계
        counterClockRotate();
    }
}

int solve() {
    // 여기에 코드를 작성해주세요.
    cin >> n >> m;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> board[i][j];
        }
    }

    while (m-- > 0) {
        // 1. 굴리기
        roll();
        // 2. 점수추가
        bfs();
        // 3. 회전
        rotateDir();
        // 4. 다시 굴리기
        roll();
    }

    cout << score << "\n";

    return 0;
}
