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
        if (!inRange(nr, nc, L)) return 0; // ����� ��� stop
        if (board[nr][nc] == 2) return 0; //  ���� ��� stop
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
    // ü���� 0���� �۾����� �����
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
        if (movable == 1) { // ���� ������ ���� ��
            // ��� ��ġ ������Ʈ
            for (auto& coord : coords) {
                int nr = coord.first + dirs[dir][0];
                int nc = coord.second + dirs[dir][1];
                board[coord.first][coord.second] -= idx; // ��ĥ�� ������
                board[nr][nc] += idx; // ��ĥ
                coord.first = nr;
                coord.second = nc;
            }
            if(depth > 0)
                computeDamage(knights[reverseKey[idx]], idx);
        }
        else { // ���� ������ ���� ��
            unordered_set<int> nexts;
            for (auto& coord : coords) {
                int nr = coord.first + dirs[dir][0];
                int nc = coord.second + dirs[dir][1];
                int next = board[nr][nc];
                if (next > 0 && next % 2 != 0) next -= 1;
                if (next > 0 && next != idx) {
                    // ��ġ�� �� ��������� ���� Ž��
                    nexts.insert(next);
                }
            }
            for(auto next : nexts)
                if (!chainReaction(knights, knights[reverseKey[next]].second, reverseKey, dir, L, next, depth+1)) return false;

            // ��������� ��� �� �Ͼ�ٸ�, ���� �� ���� �е���Ʈ
            for (auto& coord : coords) {
                int nr = coord.first + dirs[dir][0];
                int nc = coord.second + dirs[dir][1];
                board[coord.first][coord.second] -= idx; // ��ĥ�� ������
                board[nr][nc] += idx; // ��ĥ
                coord.first = nr;
                coord.second = nc;
            }
            if (depth > 0)
                computeDamage(knights[reverseKey[idx]], idx);
        }

        // ������ �� ���� ��, �� ��ȯ
        return true;
    }
}

void solve() {
    // ���⿡ �ڵ带 �ۼ����ּ���.
    ios::sync_with_stdio(0);
    cin.tie(0);

    // L : �� ����, N : ��� ����, Q : ��� ����
    int L, N, Q;
    cin >> L >> N >> Q;

    // ü���� ����
    for (int i = 1; i < L + 1; i++) {
        for (int j = 1; j < L + 1; j++) {
            int el;
            cin >> el;
            board[i][j] = el; // 0 - ��ĭ, 1 - ����, 2 - ��
        }
    }

    // �ʱ� ��� ����
    // ��� : {ü�� ,  [{x,y}]}
    vector<pair<int, vector<pair<int, int>>>> knights;
    vector<pair<int, int>> profiles;
    unordered_map<int, int> reverseKey;
    profiles.resize(N + 1);
    knights.resize(N + 1);

    int knightInd = 4;

    for (int i = 1; i <= N; i++) {
        int r, c, h, w, k; // ��, ��, ���� ����, ���� ����, ü��
        cin >> r >> c >> h >> w >> k;

        pair<int, vector<pair<int, int>>> knight;

        knight.first = k;
        for (int row = r; row < r + h; row++) {
            for (int col = c; col < c + w; col++) {
                board[row][col] += knightInd; // 4 �̻��� ������ ����� ��ȣ�� ��ĥ
                knight.second.push_back({ row, col });
            }
        }

        profiles[i] = { knightInd, k };
        knights[i] = knight;
        reverseKey[knightInd] = i;

        // �ε��� ����
        knightInd *= 2;
    }

    // ���
    for (int i = 0; i < Q; i++) {
        // idx�� �̹� ���� ����� �ε����� �־��� �� �ִ�.
        // dir : 0 - �� / 1 - �� / 2 - �� / 3 - ��
        int idx, dir; // ��� �ε���, �̵� ����
        cin >> idx >> dir;

        auto& cur = knights[idx];
        auto& health = cur.first;
        if (health <= 0) continue; // �̹� ���� ���

        auto& coords = cur.second;
        int knightNum = profiles[idx].first;
        chainReaction(knights, coords, reverseKey, dir, L, knightNum);
    }

    // ��� ���
    int res = 0;
    for (int i = 1; i <= N; i++) {
        if (knights[i].first > 0) {
            res += profiles[i].second - knights[i].first;
        }
    }
    cout << res << "\n";
}