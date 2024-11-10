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
int oldest[MAX][MAX] = { 0, }; // �������� ���� ��ž���� ���� ���� ������ 1�� ����, ������ �ߴٸ� 0���� �ʱ�ȭ
bool participate[MAX][MAX] = { 0, };
int dirs[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1,0} };
int round_dirs[8][2] = { {0, 1}, {1, 0}, {0, -1}, {-1,0}, {1,1}, {-1,-1}, {-1,1}, {1,-1} };
int N, M;
int K;

// ���ʿ� �μ����� ���� ��ž�� �ּ� 2�� �̻� �����մϴ�. + �ùķ��̼� �������� ��ΰ� �����ߴٰ� ����.

bool is_out_range(int nr, int nc) {
	return nr < 1 || nr > N || nc < 1 || nc > M;
}

// �ִܰŸ��� ��θ� return. -1�� return�ϸ� ��θ� ã�� ���� ������ ��.
pair<int, PATH> bfs(int ir, int ic, int sr, int sc, int tr, int tc) {
	// ������ ������ ������ ��, �̹� ���ϻ��·� �̵��� �� ��ǥ�� �־���.

	queue<pair<int/*���� ���� �Ÿ� cost*/, PATH /*��� ���� (top�� �̾Ƽ� ���� ��ǥ ���)*/>> nexts;
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

		// Ÿ�� ��ǥ�� ���ٸ� �ٷ� return
		if (last_vis.R == tr && last_vis.C == tc) {
			return pa;
		}

		for (auto& dir : dirs) {
			int nr = last_vis.R + dir[0];
			int nc = last_vis.C + dir[1];
			int cost = pa.first;

			// ���� �����, �ݴ������� �̵�.
			if (is_out_range(nr, nc)) {
				if (nr > N) nr = 1;
				if (nc > M) nc = 1;
				if (nr < 1) nr = N;
				if (nc < 1) nc = M;
			}


			// ������� ���� ���� ���� �ٱ��� �������.
			if (!vis[nr][nc] && board[nr][nc] != 0) { // �湮���� �ʾҰ�, �μ��� ���� �ƴ϶��.
				vis[nr][nc] = 1;
				PATH path_cpy = pa.second;
				path_cpy.push({ nr, nc }); // ���� ��ǥ�� ���� ��ܿ� ����
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
		if (res.first < min_cost) { // ���� Ž���� ��ΰ� �� �۴ٸ�
			min_cost = res.first;
			min_path = res.second;
		}
	}

	if (min_cost == INT_MAX) return false;

	//  ������ ���� ����.
	int atk_power = board[sr][sc];
	int atk_power_half = atk_power / 2; // ��

	min_path.pop(); // ��� ����
	board[tr][tc] -= atk_power; // ���߿� 0 '����' ���� üũ
	participate[tr][tc] = true;

	// ��ο� �ִ� ��ž ����.
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

			if (cur_p < power) { // ���簡 �� ���ϴٸ�
				atk = { i, j };
				power = cur_p;
				history = cur_h;
			}
			else if (cur_p == power) {
				if (cur_h < history) { // ���簡 �� �ֱٿ� �����߾��ٸ�
					atk = { i, j };
					power = cur_p;
					history = cur_h;
				}
				else if (cur_h == history) {
					if (atk.R + atk.C < i + j) { // ���� ��,���� ���� �� ũ�ٸ�
						atk = { i, j };
						power = cur_p;
						history = cur_h;
					}
					else if (atk.R + atk.C == i + j) { //
						if (atk.C < j) { // ���� ���� �� ũ�ٸ�
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

			if (cur_p > power) { // ���簡 �� ��ٸ�
				vict = { i, j };
				power = cur_p;
				history = cur_h;
			}
			else if (cur_p == power) {
				if (cur_h > history) { // ���簡 �� ��������
					vict = { i, j };
					power = cur_p;
					history = cur_h;
				}
				else if (cur_h == history) {
					if (vict.R + vict.C > i + j) { // ���� ��,���� ���� �� �۴ٸ�
						vict = { i, j };
						power = cur_p;
						history = cur_h;
					}
					else if (vict.R + vict.C == i + j) { //
						if (vict.C > j) { // ���� ���� �� �۴ٸ�
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
			if (!participate[i][j]) board[i][j] += 1; // ���ݷ� ȸ��
			if (ar != i || ac != j) oldest[i][j]++; // �޽� �߰�
		}
	}
}

void simulate()
{
	// ���� ������ �ʱ�ȭ
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			participate[i][j] = 0;
		}
	}


	// 1. ������ ����
	auto atk_coor = get_attacker();
	

	// 2. ���� ��� ����
	auto target_coor = get_target();


	// 3. ����
	board[atk_coor.R][atk_coor.C] += (N + M); // �ڵ�ĸ �ο�
	bool razor_res = razer_atk(atk_coor.R, atk_coor.C, target_coor.R, target_coor.C);
	if (!razor_res) {
		bomb_atk(atk_coor.R, atk_coor.C, target_coor.R, target_coor.C);
	}
	oldest[atk_coor.R][atk_coor.C] = 0; // ���� �ֱ� �������� ��ŷ

	// 4. �μ���
	crack();

	// 5. ����
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
