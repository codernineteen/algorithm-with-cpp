#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <array>

#define R first
#define C second

using namespace std;

/*
L x L 체스판, 상단 (1,1)
칸 구성 : 빈칸 | 함정 | 벽 (체스판 밖도 벽으로 간주)
기사 : (r,c) / 높이 x 너비의 직사각형 형태 / k 체력

1. 기사 이동 (상하좌우)
- 이동하려는 위치에 다른 기사가 있다면, 그 기사도 함께 연쇄적으로 밀림.
- 연쇄 반응 끝에 기사가 이동하려는 방향에 벽이 있으면 모든 기사는 이동 불가
- 이미 사라진 기사는 스킵

2. 대결 대미지
- 다른 기사를 밀치면 기사는 피해를 입음
    피해 : 기사가 이동한 위치의 '자신의 방패 내에 놓여있는 함정 수만큼 피해'
- 피해를 입으면 체력이 깎이고, '현재 체력 이상의 대미지'를 받는 다면 사망
- !! 명령을 받은 기사는 이동해도 피해를 입지 않음.
- 모든 기사의 이동이 끝난 후 피해를 입는다.
*/

// 0 - 빈칸, 1 - 함정, 2 - 벽
int board[41][41] = { {0,} };
int dirs[4][2] = { {-1,0}, {0,1}, {1,0}, {0, -1} };
int L, N, Q; // N - 기사, Q - 명령

pair<int, int> pos[31];
int health[31] = { 0, };
int width[31] = { 0, };
int height[31] = { 0, };
int orig[31] = { 0, };

bool isOut(int nr, int nc) {
    if (nr > L || nc > L || nr < 1 || nc < 1 || board[nr][nc] == 2) return true;
    return false;
}

vector<pair<int, int>> getSearchRange(int idx, int dir) {
    int r = pos[idx].R;
    int c = pos[idx].C;
    int w = width[idx];
    int h = height[idx];

    vector<pair<int, int>> search;

    if (dir == 0) { // 상
        r -= 1;
        for (int i = c; i <= c + w - 1; i++) {
            search.push_back({ r, i });
        }
    }
    else if (dir == 1) { // 우
        c += w;
        for (int i = r; i <= r + h - 1; i++) {
            search.push_back({ i, c });
        }
    }
    else if (dir == 2) { // 하
        r += h;
        for (int i = c; i <= c + w - 1; i++) {
            search.push_back({ r, i });
        }
    }
    else { // 좌
        c -= 1;
        for (int i = r; i <= r + h - 1; i++) {
            search.push_back({ i, c });
        }
    }

    return search;
}

vector<int> move(int idx, int dir) {
    bool needMove[31] = { 0, };
    needMove[idx] = true;

    vector<int> hitted;
    queue<int> knights;
    knights.push(idx); // 나로 시작~

    while (!knights.empty())
    {
        auto ind = knights.front();
        knights.pop();

        vector<pair<int, int>> search = getSearchRange(ind, dir);

        for (auto& next : search) {
            int nr = next.R;
            int nc = next.C;

            if (isOut(nr, nc)) return hitted;

            for (int i = 1; i <= N; i++) { // 나를 제외한 모든 기사 중 살아있는 애들
                if (i != ind && health[i] > 0) {
                    int candR = pos[i].R;
                    int candC = pos[i].C;
                    int candW = width[i];
                    int candH = height[i];

                    if (candR <= nr && nr <= candR + candH - 1 && candC <= nc && nc <= candC + candW - 1) {
                        // 겹침
                        needMove[i] = true;
                        knights.push(i);
                    }

                }
            }
        }
    }

    // 움직임이 필요한 애들 업데이트
    for (int i = 1; i <= N; i++) {
        if (needMove[i]) {
            pos[i].R += dirs[dir][0];
            pos[i].C += dirs[dir][1];
            hitted.push_back(i);
        }
    }

    return hitted;
}

void getDamage(int attacker, vector<int>& hitted) {
    for (auto& ind : hitted) {
        if (ind != attacker && health[ind] > 0) { // 내가 아니고, 살아있는 애들
            int cr = pos[ind].R;
            int cc = pos[ind].C;

            for (int r = cr; r <= cr + height[ind] - 1; r++) {
                for (int c = cc; c <= cc + width[ind] - 1; c++) {
                    if (board[r][c] == 1) { // 함정이면
                        health[ind] = max(0, health[ind] - 1);
                    }
                }
            }
        }
    }
}

int solve() {
    cin >> L >> N >> Q;

    for (int i = 1; i <= L; i++) {
        for (int j = 1; j <= L; j++) {
            cin >> board[i][j];
        }
    }

    for (int i = 1; i <= N; i++) {
        cin >> pos[i].R >> pos[i].C >> height[i] >> width[i] >> health[i];
        orig[i] = health[i]; // 점수 계산 용으로 원래 체력 기억
    }

    while (Q-- > 0) {
        // 이미 사라진 기사의 번호가 주어질 수 있다.
        int idx, dir;
        cin >> idx >> dir;

        if (health[idx] == 0) continue;

        auto hitted = move(idx, dir);
        if (hitted.empty()) continue; // 움직이지 않았음.
        getDamage(idx, hitted);
    }

    // 총 받은 대미지 합 출력
    int total = 0;
    for (int i = 1; i <= N; i++) {
        if (health[i] == 0) continue;
        total += orig[i] - health[i];
    }

    cout << total;

    return 0;
}
