#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <iostream>

using namespace std;

int board[41][41] = { {0,} };
int dirs[4][2] = {
    {-1,0}, {0,1}, {1,0}, {0, -1}
};

bool inRange(int nr, int nc, int L) {
    if (nr < 1 || nr > L || nc < 1 || nc > L) return false;
    return true;
}

int canMove(vector<pair<int, int>>& coords, int dir, int L, int idx) {
    bool interact = false;
    for (const auto& coord : coords) {
        int nr = coord.first + dirs[dir][0];
        int nc = coord.second + dirs[dir][1];
        if (!inRange(nr, nc, L)) return 0; // 장외인 경우 stop
        if (board[nr][nc] == 2) return 0; //  벽인 경우 stop
        if (board[nr][nc] > 1) {
            int next = board[nr][nc];
            if (next % 2 != 0) next -= 1;
            if (next - idx != 0) interact = true;
        }
    }
    return interact == true ? 2 : 1;
}

void computeDamage(pair<int, vector<pair<int, int>>>& knight, int knightNum) {
    for (const auto& coord : knight.second) {
        if ((board[coord.first][coord.second] - knightNum) == 1) {
            knight.first--;
        }
    }
    // 체력이 0보다 작아지면 사라짐
    if(knight.first <= 0) {
		for (const auto& coord : knight.second) {
			board[coord.first][coord.second] -= knightNum;
		}
    }
}

bool chainReaction(
    vector<pair<int, vector<pair<int, int>>>>& knights,
    vector<pair<int, int>>& coords,
    unordered_map<int, int>& reverseKey,
    int dir,
    int L,
    int idx,
    int depth = 0
) {
    int movable = canMove(coords, dir, L, idx);
    if (!movable) return false;

    else {
        if (movable == 1) { // 연쇄 반응이 없을 때
            // 기사 위치 업데이트
            for (auto& coord : coords) {
                int nr = coord.first + dirs[dir][0];
                int nc = coord.second + dirs[dir][1];
                board[coord.first][coord.second] -= idx; // 덧칠만 지워줌
                board[nr][nc] += idx; // 덧칠
                coord.first = nr;
                coord.second = nc;
            }
            if(depth > 0)
                computeDamage(knights[reverseKey[idx]], idx);
        }
        else { // 연쇄 반응이 있을 때
            unordered_set<int> nexts;
            for (auto& coord : coords) {
                int nr = coord.first + dirs[dir][0];
                int nc = coord.second + dirs[dir][1];
                int next = board[nr][nc];
                if (next > 0 && next % 2 != 0) next -= 1;
                if (next > 0 && next != idx) {
                    // 겹치는 애 재귀적으로 먼저 탐색
                    nexts.insert(next);
                }
            }
            for(auto next : nexts)
                if (!chainReaction(knights, knights[reverseKey[next]].second, reverseKey, dir, L, next, depth+1)) return false;

            // 연쇄반응이 모두 잘 일어났다면, 이제 내 상태 압데이트
            for (auto& coord : coords) {
                int nr = coord.first + dirs[dir][0];
                int nc = coord.second + dirs[dir][1];
                board[coord.first][coord.second] -= idx; // 덧칠만 지워줌
                board[nr][nc] += idx; // 덧칠
                coord.first = nr;
                coord.second = nc;
            }
            if (depth > 0)
                computeDamage(knights[reverseKey[idx]], idx);
        }

        // 움직일 수 있을 땐, 참 반환
        return true;
    }
}

void solve() {
    // 여기에 코드를 작성해주세요.
    ios::sync_with_stdio(0);
    cin.tie(0);

    // L : 판 길이, N : 기사 개수, Q : 명령 개수
    int L, N, Q;
    cin >> L >> N >> Q;

    // 체스판 정보
    for (int i = 1; i < L + 1; i++) {
        for (int j = 1; j < L + 1; j++) {
            int el;
            cin >> el;
            board[i][j] = el; // 0 - 빈칸, 1 - 함정, 2 - 벽
        }
    }

    // 초기 기사 정보
    // 기사 : {체력 ,  [{x,y}]}
    vector<pair<int, vector<pair<int, int>>>> knights;
    vector<pair<int, int>> profiles;
    unordered_map<int, int> reverseKey;
    profiles.resize(N + 1);
    knights.resize(N + 1);

    int knightInd = 4;

    for (int i = 1; i <= N; i++) {
        int r, c, h, w, k; // 행, 열, 방패 높이, 방패 넓이, 체력
        cin >> r >> c >> h >> w >> k;

        pair<int, vector<pair<int, int>>> knight;

        knight.first = k;
        for (int row = r; row < r + h; row++) {
            for (int col = c; col < c + w; col++) {
                board[row][col] += knightInd; // 4 이상의 숫자인 기사의 번호로 덧칠
                knight.second.push_back({ row, col });
            }
        }

        profiles[i] = { knightInd, k };
        knights[i] = knight;
        reverseKey[knightInd] = i;

        // 인덱스 증가
        knightInd *= 2;
    }

    // 명령
    for (int i = 0; i < Q; i++) {
        // idx는 이미 죽은 기사의 인덱스가 주어질 수 있다.
        // dir : 0 - 상 / 1 - 우 / 2 - 하 / 3 - 좌
        int idx, dir; // 기사 인덱스, 이동 방향
        cin >> idx >> dir;

        auto& cur = knights[idx];
        auto& health = cur.first;
        if (health <= 0) continue; // 이미 죽은 기사

        auto& coords = cur.second;
        int knightNum = profiles[idx].first;
        chainReaction(knights, coords, reverseKey, dir, L, knightNum);
    }

    // 결과 출력
    int res = 0;
    for (int i = 1; i <= N; i++) {
        if (knights[i].first > 0) {
            res += profiles[i].second - knights[i].first;
        }
    }
    cout << res << "\n";
}