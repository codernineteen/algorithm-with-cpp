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
int dirs[4][2] = { {-1,0}, {0,-1}, {0,1}, {1,0} }; // ��, ��, ����, �Ʒ� 

vector<pair<int, int>> convs;
umap<int, pair<int, int>> bases;
umap<int, pair<int, int>> people;

bool can_move(int r, int c)
{
	// ���� ���� ��, ���� ĭ�� 2�� �ƴҶ�.
	return (1 <= r && r <= N && 1 <= c && c <= N && board[r][c] != 2);
}

// ���� ��ǥ, Ÿ�� ��ǥ�� �Ķ���ͷ� ���� (���� ��ǥ�� �̹� ������ ������ ���� ��ǥ.)
int bfs(int sr, int sc, int tr, int tc)
{
	queue<vector<int>> nexts;
	bool vis[MAX][MAX] = { false, };
	
	vis[sr][sc] = true;
	nexts.push({ sr, sc, 1 }); // �̹� ���������� cost 1

	while (!nexts.empty()) {
		auto cur = nexts.front();
		nexts.pop();

		if (cur[0] == tr && cur[1] == tc) {
			return cur[2]; // ���� cost return
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
	// ��� ����
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

// 0 : ��ĭ , 1 : ���̽� , 2 : ��
void simulate(int time)
{
	vector<int> end_conv_list;

	for (auto& p : people) {
		// 1�� �ൿ
		move(p.first);
		// 2�� �ൿ
		if (is_conv(p.first)) end_conv_list.push_back(p.first); // �� �������� end list�� ����
	}

	// ���� ������ �������
	for (auto pid : end_conv_list) {
		board[convs[pid].R][convs[pid].C] = 2;
		people.erase(pid);
	}

	// 3�� �ൿ
	int base_id;
	if (time <= M) // t <= M�� ����.
	{
		base_id = find_base(time);
		people.insert({ time, bases[base_id] });
	}

	// �ð� ����. time�� 59.9999999��

	// time��° ����� ���� ���̽� �������
	if (time <= M) // ���ο� ����� ������ٸ�.
	{
		board[bases[base_id].R][bases[base_id].C] = 2;
		bases.erase(base_id); // �̹� ������ base ����.
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
				bases.insert({ base_id, {i, j} });// base ķ�� ���.
				base_id++;
			}
		}
	} 
	
	convs.push_back({ INT_MAX, INT_MAX }); // dummy ������ ����
	for (int i = 0; i < M; i++) {
		int r, c;
		cin >> r >> c;
		convs.push_back({ r, c }); // t�� ����� t�� ������ ��Ī.
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
