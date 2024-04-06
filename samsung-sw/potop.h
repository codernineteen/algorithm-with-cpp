//#include <bits/stdc++.h>
#include "types.h"
using namespace std;

#define X first
#define Y second

int dirs[4][2] = { {0, 1}, {1,0}, {0,-1}, {-1,0} }; // 우하좌상
int surround[8][2] = { {0, 1}, {1,0}, {0,-1}, {-1,0}, {1,1}, {1,-1}, {-1,1}, {-1,-1} };
int board[11][11] = { {0,} };
int recent[11][11] = { {0,} };
int N, M, K;
int stage = 0;
int remain = 0;

struct Node {
    int r, c;
    vector<pair<int, int>> path;
};

pair<int, int> getAttacker() {
    pair<int, int> cand;
    bool isInit = false;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            if (board[i][j] == 0) continue;
            else if (!isInit) {
                cand = { i, j };
                isInit = true;
            }
            else {
                int s1 = board[i][j];
                int s2 = board[cand.X][cand.Y];
                int r1 = recent[i][j];
                int r2 = recent[cand.X][cand.Y];

                if (s1 < s2) cand = { i, j };
                else if (s1 == s2) {
                    if (r1 > r2) cand = { i, j };
                    else if (r1 == r2) {
                        if (i + j > cand.X + cand.Y) cand = { i,j };
                        else if (i + j == cand.X + cand.Y) {
                            if (j > cand.Y) cand = { i, j };
                        }
                    }
                }
            }
        }
    }

    return cand;
}

pair<int, int> getTarget() {
    pair<int, int> cand;
    bool isInit = false;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            if (board[i][j] == 0) continue;
            else if (!isInit) {
                cand = { i, j };
                isInit = true;
            }
            else {
                int s1 = board[i][j];
                int s2 = board[cand.X][cand.Y];
                int r1 = recent[i][j];
                int r2 = recent[cand.X][cand.Y];

                if (s1 > s2) cand = { i, j };
                else if (s1 == s2) {
                    if (r1 < r2) cand = { i, j };
                    else if (r1 == r2) {
                        if (i + j < cand.X + cand.Y) cand = { i,j };
                        else if (i + j == cand.X + cand.Y) {
                            if (j < cand.Y) cand = { i, j };
                        }
                    }
                }
            }
        }
    }
    return cand;
}

bool bfs(vector<vector<int>>& vis, pair<int, int> now, pair<int, int> target, int power) {

    queue<Node> que;
    que.push(Node{ now.X, now.Y, vector<pair<int, int>>() });

    while (!que.empty()) {
        auto node = que.front();
        que.pop();

        if (node.r == target.X && node.c == target.Y) {
            vis = vector<vector<int>>(N + 1, vector<int>(M + 1, 0));
            vis[now.X][now.Y] = 1;

            for (auto cur : node.path) {
                if (cur.X == target.X && cur.Y == target.Y) board[cur.X][cur.Y] -= power;
                else board[cur.X][cur.Y] -= power / 2;

                vis[cur.X][cur.Y] = 1;
                if (board[cur.X][cur.Y] <= 0) {
                    remain--;
                    board[cur.X][cur.Y] = 0;
                }
            }
            return true;
        }

        for (auto& dir : dirs) {
            int nr = node.r + dir[0];
            int nc = node.c + dir[1];

            // 가장자리
            if (nr > N) nr = 1;
            if (nc > M) nc = 1;
            if (nr < 1) nr = N;
            if (nc < 1) nc = M;

            // 방문 or 부서짐 or 공격자
            if (vis[nr][nc] || board[nr][nc] == 0 || (nr == now.X && nc == now.Y)) continue;

            vis[nr][nc] = 1;
            vector<pair<int, int>> expandPath = node.path;
            expandPath.push_back({ nr, nc });
            que.push(Node{ nr, nc, expandPath });
        }
    }

    return false;
}

void bomb(pair<int, int> attacker, pair<int, int> target, int power, vector<vector<int>>& vis) {
    board[target.X][target.Y] -= power;
    if (board[target.X][target.Y] <= 0) {
        board[target.X][target.Y] = 0;
        remain--;
    }
    vis[target.X][target.Y] = 1;

    for (auto& sur : surround) {
        int nr = target.X + sur[0];
        int nc = target.Y + sur[1];

        // 가장자리
        if (nr > N) nr = 1;
        if (nc > M) nc = 1;
        if (nr < 1) nr = N;
        if (nc < 1) nc = M;

        if (board[nr][nc] == 0 || (nr == attacker.X && nc == attacker.Y)) continue;

        vis[nr][nc] = 1;
        board[nr][nc] -= (power / 2);
        if (board[nr][nc] <= 0) {
            board[nr][nc] = 0;
            remain--;
        }
    }
}

void attack() {
    pair<int, int> attacker = getAttacker();
    pair<int, int> target = getTarget();
    board[attacker.X][attacker.Y] += N + M; //파워 증가

    vector<vector<int>> vis(N + 1, vector<int>(M + 1, 0));
    vis[attacker.X][attacker.Y] = 1;
    bool ret = bfs(vis, attacker, target, board[attacker.X][attacker.Y]);
    if (!ret) {
        // 포탄 공격
        vis = vector<vector<int>>(N + 1, vector<int>(M + 1, 0));
        vis[attacker.X][attacker.Y] = 1;
        bomb(attacker, target, board[attacker.X][attacker.Y], vis);
    }

    // 정비
    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            if (board[i][j] == 0 || vis[i][j] == 1 || (i == attacker.X && j == attacker.Y)) continue;
            board[i][j]++;
        }
    }

    stage += 1;
    recent[attacker.X][attacker.Y] = stage;
}



int solve() {
    cin >> N >> M >> K;

    for (int i = 1; i <= N; i++) {
        for (int j = 1; j <= M; j++) {
            cin >> board[i][j];
            if (board[i][j] > 0) remain++;
        }
    }

    while (K > 0 && remain > 1) {
        attack();
        K--;
    }

    auto p = getTarget();
    cout << board[p.X][p.Y] << "\n";


    return 0;
}
