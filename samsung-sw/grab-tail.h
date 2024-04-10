#include "types.h"

#define R first
#define C second

using namespace std;

/*
n * n 격자, 3명 '이상' 한 팀. 꼬리 사람 - ..(중간 최소 한명).. - 머리 사람
주어진 이동 선을 따라서만 이동하고 (각 팀의 이동선은 끝이 이어져있따.), 이동 선은 겹치지 않는다.

1. 머리 사람을 따라서 한칸 이동
2. 각 라운드마다 공이 정해진 선을 따라 던져짐
    왼쪽 -> 오른쪽 (1 ~ n)
    아래 -> 위 (n+1 , 2n)
    오 -> 왼 (2n+1, 3n)
    위 -> 아래(3n+1, 4n)
3. 공이 던져지는 선에 사람이 있으면, '최초' 에 만나는 사람만 공을 얻어 점수를 가짐. (머리사람(1번째)으로부터 k번째 사람이면 k^2)
    공을 획득한 팀은 방향이 바로 바뀐다. (머리가 꼬리, 꼬리가 새로운 머리)


입력 : 0 - 빈칸, 1 - 머리 사람 , 2- 중간 사람, 3 - 꼬리 사람 , 4-이동선
출력 : k번의 라운드 동안 각 팀이 얻게되는 점수의 총합
*/

int n, m, k;
int board[21][21] = { {0,} };
int dirs[4][2] = { {1,0}, {0,1},{0,-1}, {-1,0} };
int score = 0;
int rounds = 1;

bool isOut(int nr, int nc) {
    return nr > n || nc > n || nr < 1 || nc < 1;
}

void move(pair<int, int> head, vector<vector<int>>& vis) {
    int backNo = 1;
    pair<int, int> back = head;

    int tempBoard[21][21] = { 0, };
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            tempBoard[i][j] = board[i][j];
        }
    }

    while (backNo != 3) { // 일단 1,2 먼저 움직이고 종료
        pair<int, int> dest;

        int tempNo;
        pair<int, int> tempBack;

        for (auto& dir : dirs) {
            int nr = back.R + dir[0];
            int nc = back.C + dir[1];

            if (isOut(nr, nc)) continue; // 범위 밖
            if (board[nr][nc] == 0) continue; // 갈수 없는 곳.

            if (backNo == 1 && board[nr][nc] == 3) {
                board[back.R][back.C] = 4; // 원래 있던 곳을 4로 마킹
                tempBoard[nr][nc] = backNo; // 다음 행선지로 옮겨놓기
                vis[nr][nc] = 1;
            }
            else {
                if (board[nr][nc] == 4) {
                    //가야할 곳
                    board[back.R][back.C] = 4; // 원래 있던 곳을 4로 마킹
                    tempBoard[nr][nc] = backNo; // 다음 행선지로 옮겨놓기
                    if (backNo == 1) vis[nr][nc] = 1;
                }
                else if (board[nr][nc] == 2) {
                    // 내 뒤에 있는 애 정보 업데이트
                    tempNo = board[nr][nc];
                    tempBack = { nr, nc };
                }
                else {
                    tempNo = 3;
                    tempBoard[back.R][back.C] = 3;
                    if (tempBoard[nr][nc] == 3) tempBoard[nr][nc] = 4;
                    tempBack = { nr, nc };
                }
            }
        }

        backNo = tempNo;
        back = tempBack;
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            board[i][j] = tempBoard[i][j];
        }
    }
}

vector<int> bfs(vector<int> start, int target) {
    vector<int> onePos;
    queue<vector<int>> que;
    que.push(start);

    vector<vector<int>> vis(n + 1, vector<int>(n + 1, 0));
    vis[start[0]][start[1]] = 1;

    while (!que.empty()) {
        vector<int> now = que.front();
        que.pop();

        if (board[now[0]][now[1]] == target) {
            onePos = now;
            break;
        }

        for (auto& dir : dirs) {
            int nr = now[0] + dir[0];
            int nc = now[1] + dir[1];
            int cost = now[2] + 1;

            if (isOut(nr, nc)) continue;
            if (vis[nr][nc]) continue;
            if (target == 1 && board[now[0]][now[1]] == 2 && board[nr][nc] == 3) continue;
            if (target == 1 && board[now[0]][now[1]] == 3 && board[nr][nc] == 1) continue;
            if (board[nr][nc] == 0 || board[nr][nc] == 4) continue;

            vis[nr][nc] = 1;
            que.push({ nr, nc, cost });
        }
    }

    return onePos;
}

pair<int, int> throwBall() {
    pair<int, int> now;
    vector<int> dir;

    // 시작점 및 방향 정하기
    if (rounds <= n) {
        now.R = rounds;
        now.C = 1;
        dir = { 0,1 };
    }
    else if (rounds > n && rounds <= 2 * n) {
        now.R = n;
        now.C = rounds - n;
        dir = { -1,0 };
    }
    else if (rounds > 2 * n && rounds <= 3 * n) {
        now.R = (n + 1) - (rounds - (2 * n));
        now.C = n;
        dir = { 0,-1 };
    }
    else if (rounds > 3 * n && rounds <= 4 * n) {
        now.R = 1;
        now.C = (n + 1) - (rounds - (3 * n));
        dir = { 1,0 };
    }

    // 공 던지기
    pair<int, int> target{ 0,0 };

    while (!isOut(now.R, now.C)) {
        if (board[now.R][now.C] > 0 && board[now.R][now.C] < 4) {
            // 잡았다 요놈
            target = { now.R, now.C };
            break;
        }
        now.R += dir[0];
        now.C += dir[1];
    }

    if (target.first == 0 && target.second == 0) return { 0,0 }; // 피격당하지 않음

    if (board[target.R][target.C] == 1) {
        score += 1;
        return target;
    }
    else { // 2나 3이 맞았을 때
        auto res = bfs(vector<int>{target.R, target.C, 1}, 1);
        score += res[2] * res[2];
        return { res[0], res[1] }; // 1의 위치
    }
}

void headToTail(pair<int, int> head) {
    auto res = bfs(vector<int>{head.R, head.C, 1}, 3);
    swap(board[res[0]][res[1]], board[head.R][head.C]);
}


int solve() {
    ios::sync_with_stdio(0);
    cin.tie(0);

    // 여기에 코드를 작성해주세요.
    cin >> n >> m >> k;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            cin >> board[i][j];
        }
    }

    while (k-- > 0) {
        // 1. 각 팀의 머리 찾기 - 완탐
        vector<vector<int>> vis(n + 1, vector<int>(n + 1, 0));
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                if (board[i][j] == 1) {
                    // 1을 찾았다면 시뮬레이션 시작.
                    if (vis[i][j] == 1) continue;
                    move({ i, j }, vis);
                }
            }
        }

        // 2. 공 던지기
        pair<int, int> head = throwBall();
        if (head.first == 0 && head.second == 0) {
            rounds++;
            if (rounds > 4 * n) rounds = 1;
        }
        else {
            headToTail(head);
            rounds++;
            if (rounds > 4 * n) rounds = 1;
        }
    }

    cout << score << "\n";

    return 0;
}
