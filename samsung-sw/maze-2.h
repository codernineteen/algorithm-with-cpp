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
int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} }; // �����¿�
int er, ec;
map<int ,pair<int, int>> people;

int ans = 0;

int get_dist(int sr, int sc) {
	return abs(sr - er) + abs(sc - ec); // |x1 - x2| + |y1 - y2|
}

bool can_move(int nr, int nc) {
	// ���� �̵� ��ǥ�� ���� ���̰�, ���� �ƴ� ��.
	return (1 <= nr && nr <= N && 1 <= nc && nc <= N) && (board[nr][nc] <= 0);
}

int move_people(int pid, int sr, int sc) {
	int min_dist = get_dist(sr, sc); // ���� ��ġ������ �Ÿ�
	pair<int, int> min_next = { 0,0 };

	for (auto& dir : dirs) {
		int nr = sr + dir[0];
		int nc = sc + dir[1];

		if (can_move(nr, nc)) {
			int n_dist = get_dist(nr, nc);
			// ���� ��ġ�� �ִܰŸ��� �� ª�ٸ�
			if (n_dist < min_dist) { // ������Ʈ
				min_dist = n_dist;
				min_next = { nr, nc };
			}
		}
	}

	if (min_next.R == 0 && min_next.C == 0) return INT_MAX; // �ѹ��� ������Ʈ�� ������. ������ �� ����.

	// �̵�
	people[pid] = min_next;
	ans += 1; // �̵��� �߻�������, �̵� �Ÿ� 1�߰�

	if (min_next.R == er && min_next.C == ec) return pid;

	return INT_MAX;
}

pair<int, pair<int, int>> find_min_square() {
	int min_len = INT_MAX;
	pair<int, int> min_sq = { 0, 0 };
	vector<int> info = {INT_MAX, 0, 0};

	for (int len = 1; len <= N; len++) {
		// ��ǥ�� ���� ���簢�� �� ���� ���̸� �� Ž���� �� �ִ� �� ������ Ž��.
		for (int i = 1; i <= N - len + 1; i++) {
			for (int j = 1; j <= N - len + 1; j++) {
				bool is_exit = false;
				bool is_person = false;

				// ���� ��ġ���� len�� �ش��ϴ� ���簢��.
				for (int sr = i; sr < i + len; sr++) {
					for (int sc = j; sc < j + len; sc++) {
						if (board[sr][sc] == -1) is_exit = true;
						for (auto& p : people) {
							if (p.second.R == sr && p.second.C == sc) is_person = true;
						}
					}
				}

				// ����� �־���, �ⱸ�� �־�����.
				if (is_exit && is_person) {
					if (len < min_len) { // ���� ���̰� �� ª����.
						min_len = len; // ���� ���簢�� ����
						min_sq = { i,j }; // ���� ���簢���� �»��
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
			int value = board[i][j]; // ���� ����� ��

			// �������� �̵�.
			int ir = i - sr;
			int ic = j - sc;

			// ȸ�� ��ǥ
			int nr = ic;
			int nc = len - ir - 1;
			cp_board[nr][nc] = value;

			// ���� ��ǥ�� ���?
			for (auto& p : people) {
				auto coor = p.second;
				// ȸ����Ų ���� ����, ��ǥ�� �����ϴٸ�
				if (rotated_p.find(p.first) == rotated_p.end() && coor.R == i && coor.C == j) {
					p.second = { nr + sr, nc + sc }; // ȸ���� ��ǥ�� ����
					rotated_p.insert(p.first);
				}
			}

			// ���� ��ǥ�� Ż�ⱸ?
			if (!rotated_e && board[i][j] == -1) {
				// ȸ���� Ż�ⱸ.
				er = nr + sr;
				ec = nc + sc;
				rotated_e = true;
			}
		}
	}

	// ���� ���忡 �ٽ� ����.
	for (int i = sr; i < sr + len; i++) {
		for (int j = sc; j < sc + len; j++) {
			// cp_board�� ���� ���� ��ǥ
			int ir = i - sr;
			int ic = j - sc;

			board[i][j] = cp_board[ir][ic];
			if (board[i][j] > 0) board[i][j] -= 1; // �������� �ִ� ���̸� ����.
		}
	}
}

bool simulate() {

	// 1�� ���� ��� �̵�.
	vector<int> exit_list;
	for (auto& person : people) {
		int res = move_people(person.first, person.second.R, person.second.C);
		if (res != INT_MAX) exit_list.push_back(res);
	}

	for (auto& pid : exit_list) people.erase(pid);

	if (people.empty()) return true; // ��� �̵� �Ŀ� ����� ������ Ȯ��.

	// �̵� ��, �̷� ȸ��.
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

	// �� ���
	cout << ans << "\n" << er << " " << ec << "\n";

	return 0;
}
