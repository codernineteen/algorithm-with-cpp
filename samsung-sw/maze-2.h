#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>
#include <map>
#include <unordered_set>

#define R first
#define C second
#define MAX 11

using namespace std;

int N, M, K;
int board[MAX][MAX] = { 0, };
int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} }; // 상하좌우
int er, ec;
map<int ,pair<int, int>> people;

int ans = 0;

int get_dist(int sr, int sc) {
	return abs(sr - er) + abs(sc - ec); // |x1 - x2| + |y1 - y2|
}

bool can_move(int nr, int nc) {
	// 다음 이동 좌표가 보드 안이고, 벽이 아닐 때.
	return (1 <= nr && nr <= N && 1 <= nc && nc <= N) && (board[nr][nc] <= 0);
}

int move_people(int pid, int sr, int sc) {
	int min_dist = get_dist(sr, sc); // 지금 위치에서의 거리
	pair<int, int> min_next = { 0,0 };

	for (auto& dir : dirs) {
		int nr = sr + dir[0];
		int nc = sc + dir[1];

		if (can_move(nr, nc)) {
			int n_dist = get_dist(nr, nc);
			// 다음 위치의 최단거리가 더 짧다면
			if (n_dist < min_dist) { // 업데이트
				min_dist = n_dist;
				min_next = { nr, nc };
			}
		}
	}

	if (min_next.R == 0 && min_next.C == 0) return INT_MAX; // 한번도 업데이트가 없었음. 움직일 수 없음.

	// 이동
	people[pid] = min_next;
	ans += 1; // 이동이 발생했으면, 이동 거리 1추가

	if (min_next.R == er && min_next.C == ec) return pid;

	return INT_MAX;
}

pair<int, pair<int, int>> find_min_square() {
	int min_len = INT_MAX;
	pair<int, int> min_sq = { 0, 0 };
	vector<int> info = {INT_MAX, 0, 0};

	for (int len = 1; len <= N; len++) {
		// 좌표는 지금 정사각형 한 변의 길이를 다 탐색할 수 있는 곳 까지만 탐색.
		for (int i = 1; i <= N - len + 1; i++) {
			for (int j = 1; j <= N - len + 1; j++) {
				bool is_exit = false;
				bool is_person = false;

				// 지금 위치에서 len에 해당하는 정사각형.
				for (int sr = i; sr < i + len; sr++) {
					for (int sc = j; sc < j + len; sc++) {
						if (board[sr][sc] == -1) is_exit = true;
						for (auto& p : people) {
							if (p.second.R == sr && p.second.C == sc) is_person = true;
						}
					}
				}

				// 사람도 있었고, 출구도 있었으면.
				if (is_exit && is_person) {
					if (len < min_len) { // 현재 길이가 더 짧으면.
						min_len = len; // 현재 정사각형 길이
						min_sq = { i,j }; // 현재 정사각형의 좌상단
					}
				}
			}
		}
	}

	return { min_len, min_sq };
}

void rotate_square(int len, int sr, int sc) {
	int cp_board[MAX][MAX] = { 0, };
	unordered_set<int> rotated_p;
	bool rotated_e = false;

	for (int i = sr; i < sr + len; i++) {
		for (int j = sc; j < sc + len; j++) {
			int value = board[i][j]; // 원래 보드상 값

			// 원점으로 이동.
			int ir = i - sr;
			int ic = j - sc;

			// 회전 좌표
			int nr = ic;
			int nc = len - ir - 1;
			cp_board[nr][nc] = value;

			// 지금 좌표가 사람?
			for (auto& p : people) {
				auto coor = p.second;
				// 회전시킨 적이 없고, 좌표가 동일하다면
				if (rotated_p.find(p.first) == rotated_p.end() && coor.R == i && coor.C == j) {
					p.second = { nr + sr, nc + sc }; // 회전한 좌표로 변경
					rotated_p.insert(p.first);
				}
			}

			// 지금 좌표가 탈출구?
			if (!rotated_e && board[i][j] == -1) {
				// 회전된 탈출구.
				er = nr + sr;
				ec = nc + sc;
				rotated_e = true;
			}
		}
	}

	// 원래 보드에 다시 복사.
	for (int i = sr; i < sr + len; i++) {
		for (int j = sc; j < sc + len; j++) {
			// cp_board의 원점 기준 좌표
			int ir = i - sr;
			int ic = j - sc;

			board[i][j] = cp_board[ir][ic];
			if (board[i][j] > 0) board[i][j] -= 1; // 내구도가 있는 벽이면 가감.
		}
	}
}

bool simulate() {

	// 1초 동안 모두 이동.
	vector<int> exit_list;
	for (auto& person : people) {
		int res = move_people(person.first, person.second.R, person.second.C);
		if (res != INT_MAX) exit_list.push_back(res);
	}

	for (auto& pid : exit_list) people.erase(pid);

	if (people.empty()) return true; // 모두 이동 후에 사람이 없는지 확인.

	// 이동 끝, 미로 회전.
	auto sq_info = find_min_square();
	int sq_len = sq_info.first;
	pair<int, int> sq_top = sq_info.second;
	rotate_square(sq_len, sq_top.R, sq_top.C);


	return false;
}


int solve()
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M >> K;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> board[i][j];
		}
	}

	for (int id = 0; id < M; id++) {
		int pr, pc;
		cin >> pr >> pc;
		people.insert({id, { pr, pc }});
	}

	cin >> er >> ec;
	board[er][ec] = -1;

	int time = K;
	while (K-- > 0) {
		bool res = simulate();
		if (res) break;
	}

	// 답 출력
	cout << ans << "\n" << er << " " << ec << "\n";

	return 0;
}
