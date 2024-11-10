#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>
#include <stack>
#include <unordered_map>
#include <unordered_set>

using namespace std;

#define MIN_B 1
#define MAX_B 4
#define ROW(x) x[0]
#define COL(x) x[1]
#define DIR(x) x[2]

int p_dirs[4][2] = { {-1,0}, {0,-1}, {1,0}, {0,1} }; // 상좌하우
int m_dirs[8][2] = { {-1,0}, {-1,-1}, {0,-1}, {1,-1}, {1,0}, {1,1}, {0,1}, {-1,1} }; // 상, 상좌, 좌, 좌하, 하, 우하, 우, 우상

struct Mon {
	int r, c, dir;
};

int M, T;

int MON = 0;

vector<int> pac;
unordered_map<int, Mon> mons;
int mon_board[5][5] = { 0, };
int deads[5][5] = { 0, };
stack<Mon> eggs;

void replicate() {
	for (auto& mon : mons)
	{
		auto m = mon.second;
		eggs.push(Mon{ m.r, m.c, m.dir });
	}
}

void born() {
	while (!eggs.empty()) {
		auto egg = eggs.top();
		eggs.pop();

		mons.insert({ MON++, Mon{egg.r, egg.c, egg.dir} });
		mon_board[egg.r][egg.c]++;
	}
}

bool in_range(int r, int c) {
	return (MIN_B <= r && r <= MAX_B && MIN_B <= c && c <= MAX_B);
}

void clean_dead() {
	for (int i = 1; i <= 4; i++)
	{
		for (int j = 1; j <= 4; j++) {
			deads[i][j] = max(deads[i][j]-1, 0);
		}
	}
}

bool can_move(int nr, int nc) {
	// 범위 바깥이면
	if (!in_range(nr, nc)) return false;
	// 팩맨과 겹치면
	if (nr == ROW(pac) && nc == COL(pac)) return false;
	// 시체와 겹치면
	if (deads[nr][nc] > 0) return false;

	return true;
}

void move_mon(int mid) {
	auto& m = mons[mid];

	int dir = m.dir;
	for (int i = 0; i < 8; i++) {
		int nr = m.r + m_dirs[dir][0];
		int nc = m.c + m_dirs[dir][1];

		if (can_move(nr, nc)) {
			mon_board[m.r][m.c]--;

			m.r = nr;
			m.c = nc;
			m.dir = dir;

			mon_board[nr][nc]++;

			break;
		}

		// 반시계 45도 회전
		dir = (dir + 1) % 8;
	}
}

void move_pac() {
	vector<pair<int, int>> route;
	int max_ate = INT_MIN;

	for (int i = 0; i < 4; i++) {

		int r1 = ROW(pac) + p_dirs[i][0];
		int c1 = COL(pac) + p_dirs[i][1];
		if (!in_range(r1, c1)) continue;

		for (int j = 0; j < 4; j++) {

			int r2 = r1 + p_dirs[j][0];
			int c2 = c1 + p_dirs[j][1];
			if (!in_range(r2, c2)) continue;

			for (int k = 0; k < 4; k++) {
				int cur_ate = 0;

				int r3 = r2 + p_dirs[k][0];
				int c3 = c2 + p_dirs[k][1];
				if (!in_range(r3, c3)) continue;

				if (r1 == r3 && c1 == c3) {
					cur_ate = (mon_board[r1][c1] + mon_board[r2][c2]);
				}
				else {
					cur_ate = (mon_board[r1][c1] + mon_board[r2][c2] + mon_board[r3][c3]);
				}

				// update
				if (cur_ate > max_ate) {
					max_ate = cur_ate;
					route = { {r1, c1},{r2, c2}, {r3, c3} };
				}
			}
		}
	}

	// 최고 조합 찾았음. 이동 및 시체처리
	ROW(pac) = route[2].first;
	COL(pac) = route[2].second;

	vector<int> ate_mons;
	for (auto& m : mons) {
		if (
			(route[0].first == m.second.r && route[0].second == m.second.c) ||
			(route[1].first == m.second.r && route[1].second == m.second.c) ||
			(route[2].first == m.second.r && route[2].second == m.second.c)
			)
		{
			ate_mons.push_back(m.first);
		}
	}

	for (auto& ate : ate_mons) {
		int r = mons[ate].r;
		int c = mons[ate].c;
		mons.erase(ate); // 살아있는 몬스터에서 지우기
		deads[r][c] = 3;
		mon_board[r][c]--;
	}
}

void simulate() {
	replicate();

	for (auto& m : mons) {
		move_mon(m.first);
	}

	move_pac();

	clean_dead();

	born();
}

int solve()
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> M >> T;

	pac.resize(2);
	cin >> ROW(pac) >> COL(pac);

	for (int i = 0; i < M; i++) {
		int r, c, dir;
		cin >> r >> c >> dir;
		mon_board[r][c] += 1;
		mons.insert({ MON++,Mon{ r, c, dir - 1 /*1부터 시작해서 1줄이고 넣기*/} });
	}

	while (T-- > 0) {
		simulate();
	}

	cout << mons.size() << "\n";

	return 0;
}
