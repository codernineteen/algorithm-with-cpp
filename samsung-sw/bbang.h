#include "types.h"
// 격자 n x n
// m명의 빵을 원하는 사람, 1번 사람 1분 출발 -> M까지 반복
// 각자의 베이스 캠프에서 출발하고, 출발 전에는 격자 바깥에서 대기
// 각자가 목표하는 편의점이 '모두' 다름

// 행동 원칙 1 -> 2 -> 3
/*
각 행동은 1분간 모두 이루어진다. 격자 내에 사람이 없을 때는 3번-베이스 캠프 찾기 부터 진행.
1. 가고 싶은 편의점 향해서 1칸(우선순위 : 상좌우하)
2. 편의점에 도착하면 멈춘다. 만약 편의점에 도착하면 다음 라운드부터 그 칸을 지나갈 수 없다.
3. 현재 시간 t(<=m) 일 때, t번 사람은 편의점과 가장 가까이 있는 베이스 캠프에 진입.
   기준 : 최단거리(맨해튼 거리) -> 같을 때 행이 작은 -> 같을 때 열이 작은
   베이스 캠프까지 이동에는 시간 소요 없음
   다른 사람은 t시점부터 베이스 캠프를 '영원히' 지나갈 수 없음
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
    // 도착하면 people에 해당 사람 (0,0) 업데이트 / 보드 상태 -1로 업데이트
    int curR = people[pInd][0];
    int curC = people[pInd][1];
    int tr = dests[pInd][0];
    int tc = dests[pInd][1];
    if (curR == tr && curC == tc) { // 도착
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
            board[dests[e][0]][dests[e][1]] = -1; // 점령된 편의점
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
