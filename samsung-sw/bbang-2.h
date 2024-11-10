#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <climits>

using namespace std;

#define R first
#define C second

#define MAX 16
#define umap unordered_map

int N, M;
int board[MAX][MAX] = { 0, };
int dirs[4][2] = { {-1,0}, {0,-1}, {0,1}, {1,0} }; // 위, 왼, 오른, 아래 

vector<pair<int, int>> convs;
umap<int, pair<int, int>> bases;
umap<int, pair<int, int>> people;

bool can_move(int r, int c)
{
	// 보드 범위 내, 다음 칸이 2가 아닐때.
	return (1 <= r && r <= N && 1 <= c && c <= N && board[r][c] != 2);
}

// 시작 좌표, 타겟 좌표를 파라미터로 전달 (시작 좌표는 이미 방향이 더해진 후의 좌표.)
int bfs(int sr, int sc, int tr, int tc)
{
	queue<vector<int>> nexts;
	bool vis[MAX][MAX] = { false, };
	
	vis[sr][sc] = true;
	nexts.push({ sr, sc, 1 }); // 이미 움직였으니 cost 1

	while (!nexts.empty()) {
		auto cur = nexts.front();
		nexts.pop();

		if (cur[0] == tr && cur[1] == tc) {
			return cur[2]; // 누적 cost return
		}

		for (auto& dir : dirs) {
			int nr = cur[0] + dir[0];
			int nc = cur[1] + dir[1];
			
			if (can_move(nr, nc) && !vis[nr][nc]) {
				vis[nr][nc] = true;
				nexts.push({ nr, nc, cur[2] + 1 });
			}
		}
	}

	return INT_MAX;
}

void move(int pid)
{
	int sr = people[pid].R;
	int sc = people[pid].C;
	int tr = convs[pid].R;
	int tc = convs[pid].C;

	int min_cost = INT_MAX;
	pair<int, int> min_next;

	for (auto& dir : dirs) {
		int nr = sr + dir[0];
		int nc = sc + dir[1];
		if (can_move(nr, nc)) {
			int cost = bfs(nr, nc, tr, tc);
			if (cost < min_cost) {
				min_cost = cost;
				min_next = { nr, nc };
			}
		}
	}

	people[pid].R = min_next.R;
	people[pid].C = min_next.C;
}

bool is_conv(int pid)
{
	auto coor1 = people[pid];
	auto coor2 = convs[pid];

	return coor1.R == coor2.R && coor1.C == coor2.second;
}

int find_base(int pid)
{
	// 사람 생성
	int tr = convs[pid].R;
	int tc = convs[pid].C;

	int min_cost = INT_MAX;
	int base_id = -1;

	for (auto& base : bases) {
		int cost = bfs(base.second.R, base.second.C, tr, tc);

		if (cost < min_cost) {
			min_cost = cost;
			base_id = base.first;
		}
		else if (cost == min_cost) {
			if (bases[base_id].R > base.second.R) {
				min_cost = cost;
				base_id = base.first;
			}
			else if (bases[base_id].R == base.second.R) {
				if (bases[base_id].C > base.second.C) {
					min_cost = cost;
					base_id = base.first;
				}
			}
		}
		
	}

	return base_id;
}

// 0 : 빈칸 , 1 : 베이스 , 2 : 벽
void simulate(int time)
{
	vector<int> end_conv_list;

	for (auto& p : people) {
		// 1번 행동
		move(p.first);
		// 2번 행동
		if (is_conv(p.first)) end_conv_list.push_back(p.first); // 들어간 편의점을 end list에 삽입
	}

	// 끝난 편의점 통행금지
	for (auto pid : end_conv_list) {
		board[convs[pid].R][convs[pid].C] = 2;
		people.erase(pid);
	}

	// 3번 행동
	int base_id;
	if (time <= M) // t <= M일 때만.
	{
		base_id = find_base(time);
		people.insert({ time, bases[base_id] });
	}

	// 시간 종료. time분 59.9999999초

	// time번째 사람이 들어온 베이스 통행금지
	if (time <= M) // 새로운 사람이 생겼었다면.
	{
		board[bases[base_id].R][bases[base_id].C] = 2;
		bases.erase(base_id); // 이미 점유한 base 삭제.
	}
}

int solve()
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> N >> M;

	int base_id = 0;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> board[i][j];
			if (board[i][j] == 1) {
				bases.insert({ base_id, {i, j} });// base 캠프 기억.
				base_id++;
			}
		}
	} 
	
	convs.push_back({ INT_MAX, INT_MAX }); // dummy 편의점 삽입
	for (int i = 0; i < M; i++) {
		int r, c;
		cin >> r >> c;
		convs.push_back({ r, c }); // t번 사람과 t번 편의점 매칭.
	}

	int t = 1;
	while (true)
	{
		simulate(t);
		if (t != 1 && people.empty()) break;
		t++;
	}

	cout << t << "\n";


	return 0;
}
