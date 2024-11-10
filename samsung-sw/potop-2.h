#include <iostream>
#include <queue>
#include <stack>
#include <vector>
#include <map>
#include <climits>
#include <algorithm>

using namespace std;

#define MAX 11
#define R first
#define C second
#define PATH stack<pair<int, int>>

int board[MAX][MAX] = { 0, };
int oldest[MAX][MAX] = { 0, }; // 공격하지 않은 포탑들은 턴이 끝날 때마다 1씩 증가, 공격을 했다면 0으로 초기화
bool participate[MAX][MAX] = { 0, };
int dirs[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1,0} };
int round_dirs[8][2] = { {0, 1}, {1, 0}, {0, -1}, {-1,0}, {1,1}, {-1,-1}, {-1,1}, {1,-1} };
int N, M;
int K;

// 최초에 부서지지 않은 포탑은 최소 2개 이상 존재합니다. + 시뮬레이션 이전에는 모두가 공격했다고 가정.

bool is_out_range(int nr, int nc) {
	return nr < 1 || nr > N || nc < 1 || nc > M;
}

// 최단거리와 경로를 return. -1을 return하면 경로를 찾지 못한 것으로 함.
pair<int, PATH> bfs(int ir, int ic, int sr, int sc, int tr, int tc) {
	// 레이저 어택을 시작할 때, 이미 우하상좌로 이동한 후 좌표가 주어짐.

	queue<pair<int/*현재 까지 거리 cost*/, PATH /*경로 스택 (top을 뽑아서 다음 좌표 계산)*/>> nexts;
	bool vis[MAX][MAX] = { 0, };

	vis[sr][sc] = 1;
	vis[ir][ic] = 1;
	PATH init_path;
	init_path.push({ sr, sc });
	nexts.push({ 1, init_path });

	while (!nexts.empty()) {
		pair<int, PATH> pa = nexts.front();
		nexts.pop();
		pair<int, int> last_vis = pa.second.top();

		// 타겟 좌표와 같다면 바로 return
		if (last_vis.R == tr && last_vis.C == tc) {
			return pa;
		}

		for (auto& dir : dirs) {
			int nr = last_vis.R + dir[0];
			int nc = last_vis.C + dir[1];
			int cost = pa.first;

			// 범위 벗어나면, 반대편으로 이동.
			if (is_out_range(nr, nc)) {
				if (nr > N) nr = 1;
				if (nc > M) nc = 1;
				if (nr < 1) nr = N;
				if (nc < 1) nc = M;
			}


			// 여기까지 오면 절대 범위 바깥은 없어야함.
			if (!vis[nr][nc] && board[nr][nc] != 0) { // 방문하지 않았고, 부서진 곳이 아니라면.
				vis[nr][nc] = 1;
				PATH path_cpy = pa.second;
				path_cpy.push({ nr, nc }); // 다음 좌표를 스택 상단에 넣음
				nexts.push({ cost + 1, path_cpy });
			}
		}

	}

	return { INT_MAX, PATH{} };
}

bool razer_atk(int sr, int sc, int tr, int tc) {

	int min_cost = INT_MAX;
	PATH min_path;

	for (auto& dir : dirs) {
		int nr = sr + dir[0];
		int nc = sc + dir[1];

		if (is_out_range(nr, nc)) {
			if (nr > N) nr = 1;
			if (nc > M) nc = 1;
			if (nr < 1) nr = N;
			if (nc < 1) nc = M;
		}
		if (board[nr][nc] == 0) continue;

		pair<int, PATH> res = bfs(sr, sc, nr, nc, tr, tc);
		if (res.first < min_cost) { // 새로 탐색한 경로가 더 작다면
			min_cost = res.first;
			min_path = res.second;
		}
	}

	if (min_cost == INT_MAX) return false;

	//  레이저 공격 시작.
	int atk_power = board[sr][sc];
	int atk_power_half = atk_power / 2; // 몫

	min_path.pop(); // 상단 제거
	board[tr][tc] -= atk_power; // 나중에 0 '이하' 인지 체크
	participate[tr][tc] = true;

	// 경로에 있는 포탑 공격.
	while (!min_path.empty()) {
		auto cur = min_path.top(); min_path.pop();
		board[cur.R][cur.C] -= atk_power_half;
		participate[cur.R][cur.C] = true;
	}

	return true;
}

void bomb_atk(int sr, int sc, int tr, int tc) {
	int atk_power = board[sr][sc];
	int atk_power_half = atk_power / 2;

	board[tr][tc] -= atk_power;
	participate[tr][tc] = true;

	for (auto& dir : round_dirs) {
		int nr = tr + dir[0];
		int nc = tc + dir[1];

		if (nr > N) nr = 1;
		if (nc > M) nc = 1;
		if (nr < 1) nr = N;
		if (nc < 1) nc = M;

		if (board[nr][nc] == 0) continue;

		if (nr != sr || nc != sc)
		{
			board[nr][nc] -= atk_power_half;
		}

		participate[nr][nc] = true;
	}
}

pair<int, int> get_attacker()
{
	pair<int, int> atk;
	int power = INT_MAX;
	int history = INT_MAX;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (board[i][j] == 0) continue;

			int cur_p = board[i][j];
			int cur_h = oldest[i][j];

			if (cur_p < power) { // 현재가 더 약하다면
				atk = { i, j };
				power = cur_p;
				history = cur_h;
			}
			else if (cur_p == power) {
				if (cur_h < history) { // 현재가 더 최근에 공격했었다면
					atk = { i, j };
					power = cur_p;
					history = cur_h;
				}
				else if (cur_h == history) {
					if (atk.R + atk.C < i + j) { // 현재 행,열의 합이 더 크다면
						atk = { i, j };
						power = cur_p;
						history = cur_h;
					}
					else if (atk.R + atk.C == i + j) { //
						if (atk.C < j) { // 현재 열이 더 크다면
							atk = { i, j };
							power = cur_p;
							history = cur_h;
						}
					}
				}
			}
		}
	}

	return atk;
}

pair<int, int> get_target()
{
	pair<int, int> vict;
	int power = INT_MIN;
	int history = INT_MIN;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (board[i][j] == 0) continue;

			int cur_p = board[i][j];
			int cur_h = oldest[i][j];

			if (cur_p > power) { // 현재가 더 쎄다면
				vict = { i, j };
				power = cur_p;
				history = cur_h;
			}
			else if (cur_p == power) {
				if (cur_h > history) { // 현재가 더 오래전에
					vict = { i, j };
					power = cur_p;
					history = cur_h;
				}
				else if (cur_h == history) {
					if (vict.R + vict.C > i + j) { // 현재 행,열의 합이 더 작다면
						vict = { i, j };
						power = cur_p;
						history = cur_h;
					}
					else if (vict.R + vict.C == i + j) { //
						if (vict.C > j) { // 현재 열이 더 작다면
							vict = { i, j };
							power = cur_p;
							history = cur_h;
						}
					}
				}
			}
		}
	}

	return vict;
}

bool is_end() {
	int count = 0;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (board[i][j] > 0) count++;
		}
	}

	return count == 1;
}

void crack()
{
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (board[i][j] <= 0) board[i][j] = 0;
		}
	}
}

void fix_top(int ar, int ac)
{
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			if (board[i][j] == 0) continue;
			if (!participate[i][j]) board[i][j] += 1; // 공격력 회복
			if (ar != i || ac != j) oldest[i][j]++; // 휴식 추가
		}
	}
}

void simulate()
{
	// 공격 참여자 초기화
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			participate[i][j] = 0;
		}
	}


	// 1. 공격자 선정
	auto atk_coor = get_attacker();
	

	// 2. 공격 대상 선정
	auto target_coor = get_target();


	// 3. 공격
	board[atk_coor.R][atk_coor.C] += (N + M); // 핸디캡 부여
	bool razor_res = razer_atk(atk_coor.R, atk_coor.C, target_coor.R, target_coor.C);
	if (!razor_res) {
		bomb_atk(atk_coor.R, atk_coor.C, target_coor.R, target_coor.C);
	}
	oldest[atk_coor.R][atk_coor.C] = 0; // 가장 최근 공격으로 마킹

	// 4. 부수기
	crack();

	// 5. 정비
	participate[atk_coor.R][atk_coor.C] = true;
	participate[target_coor.R][target_coor.C] = true;
	fix_top(atk_coor.R, atk_coor.C);
}



int solve()
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M >> K;

	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			cin >> board[i][j];
		}
	}

	int cnt = K;
	while (cnt-- > 0) {
		if (is_end()) break;
		simulate();
	}

	int max_power = INT_MIN;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) max_power = max(board[i][j], max_power);
	}
	cout << max_power << "\n";

	return 0;
}
