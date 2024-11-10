#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Knight
{
	int r, c;
	int h, w;
	int k; // health
};

int L, N, Q;
vector<Knight> k_arr;
vector<Knight> init_k_arr; // �� ������ , ���� x
int board[41][41] = { 0, };
int dirs[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1} }; // ��, ����, �Ʒ�, ��

vector<int> is_overlapped(int id, int d, int r, int c, int h, int w)
{
	vector<int> overlapped;
	for (int i = 0; i < N; i++) // knight iter
	{
		if (id == i || k_arr[i].k <= 0) continue; // ������ ����̰ų� �̹� ���� ����̸� skip
		else
		{
			int r1 = k_arr[i].r;
			int c1 = k_arr[i].c;
			int r2 = k_arr[i].r + k_arr[i].h - 1;
			int c2 = k_arr[i].c + k_arr[i].w - 1;;
			if (d == 0) // �� �̵�
			{
				if (
					r <= r2 && r2 <= r + h - 1 && (
					(c <= c1 && c1 <= c + w - 1) ||
					(c <= c2 && c2 <= c + w - 1) ||
					(c1 < c && c + w - 1 < c2))
					) overlapped.push_back(i);
			}
			else if (d == 1) // ���� �̵�
			{
				if (
					c <= c1 && c1 <= c + w - 1 && (
					(r <= r1 && r1 <= r + h - 1) ||
					(r <= r2 && r2 <= r + h - 1) ||
					(r1 < r && r + h - 1 < r2))
					) overlapped.push_back(i);

			}
			else if(d == 2) // �Ʒ� �̵�
			{
				if (
					r <= r1 && r1 <= r + h - 1 && (
					(c <= c1 && c1 <= c + w - 1) ||
					(c <= c2 && c2 <= c + w - 1) ||
					(c1 < c && c + w - 1 < c2))
					) overlapped.push_back(i);
			}
			else // �� �̵�
			{ 
				if (
					c <= c2 && c2 <= c + w - 1 && (
					(r <= r1 && r1 <= r + h - 1) ||
					(r <= r2 && r2 <= r + h - 1) ||
					(r1 < r && r + h - 1 < r2))
					) overlapped.push_back(i);
			}
		}
	}

	return overlapped;
}
//
//vector<int> is_overlapped(int id, int d, int r, int c, int h, int w)
//{
//	vector<int> overlapped;
//	for (int i = 0; i < N; i++) // knight iter
//	{
//		if (id == i || k_arr[i].k <= 0) continue; // ������ ����̰ų� �̹� ���� ����̸� skip
//		for(int sr = r; sr < r + h; sr++)
//		{
//			for(int sc = c; sc < c + w; sc++)
//			{
//				int r1 = k_arr[i].r;
//				int c1 = k_arr[i].c;
//				int h1 = k_arr[i].h;
//				int w1 = k_arr[i].w;
//				bool find = false;
//				for (int sr2 = r1; sr2 < r1 + h1; sr2++)
//				{
//					for (int sc2 = c1; sc2 < c1 + w1; sc2++)
//					{
//						if (sr == sr2 && sc == sc2) {
//							overlapped.push_back(i);
//							find = true;
//							break;
//						}
//					}
//					if (find) break;
//				}
//			}
//		}
//	}
//
//	return overlapped;
//}

bool is_wall(int r, int c, int h, int w)
{
	// ���� �ٱ� üũ
	if (r < 1 or r+h-1 > L or c < 1 or c+w-1 > L) return true;

	// ���� �� üũ
	for (int sr = r; sr < r + h; sr++)
	{
		for (int sc = c; sc < c + w; sc++)
		{
			if (board[sr][sc] == 2) return true;
		}
	}

	return false;
}

void move_and_damage(int id, int d)
{
	// move
	k_arr[id].r += dirs[d][0];
	k_arr[id].c += dirs[d][1];

	for (int sr = k_arr[id].r; sr < k_arr[id].r + k_arr[id].h; sr++)
	{
		for (int sc = k_arr[id].c; sc < k_arr[id].c + k_arr[id].w; sc++)
		{
			if (board[sr][sc] == 1) k_arr[id].k -= 1; // ü�� ����
		}
	}
}

//void print_board()
//{
//	vector<vector<int>> board(L+1, vector<int>(L+1, 0));
//
//	int id = 1;
//	for (auto& k : k_arr)
//	{
//		if (k.k >= 0)
//		{
//			for (int r = k.r; r < k.r + k.h; r++) {
//				for (int c = k.c; c < k.c + k.w; c++) {
//					board[r][c] = id;
//				}
//			}
//		}
//
//		id++;
//	}
//
//	for (int i = 1; i <= L; i++)
//	{
//		for (int j = 1; j <= L; j++)
//		{
//			cout << board[i][j] << " ";
//		}
//		cout << "\n";
//	}
//}

void simulate(int id, int d)
{
	//print_board();
	int nr, nc, height, width;

	bool hit_wall = false;
	int cur_id = id;

	queue<int> trace;
	queue<int> nexts;
	nexts.push(id);
	trace.push(id);
	vector<bool> vis(N, 0);
	vis[id] = 1;

	// bfs
	while (!nexts.empty())
	{
		int cur_id = nexts.front(); 
		nexts.pop();

		nr = k_arr[cur_id].r + dirs[d][0];
		nc = k_arr[cur_id].c + dirs[d][1];
		height = k_arr[cur_id].h;
		width = k_arr[cur_id].w;

		if (is_wall(nr, nc, height, width)) {
			hit_wall = true;
			break;
		}

		vector<int> next_ids = is_overlapped(cur_id, d, nr, nc, height, width);
		for (auto& n : next_ids) {
			if (!vis[n])
			{
				vis[n] = 1;
				trace.push(n);
				nexts.push(n);
			}
		}
	}

	if (hit_wall) return; // ���� �ƴٸ� �ƹ� �ϵ� ����.
	
	while (!trace.empty())
	{
		int t_id = trace.front();
		trace.pop();
		if (t_id == id) // ��ɹ��� ����� �����̱⸸
		{
			k_arr[id].r += dirs[d][0];
			k_arr[id].c += dirs[d][1];
		}
		else
		{
			move_and_damage(t_id, d);
		}
	}
}

int solve()
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> L >> N >> Q;

	// ü���� ����
	for (int i = 1; i <= L; i++)
	{
		for (int j = 1; j <= L; j++)
		{
			cin >> board[i][j];
		}
	}
	

	// �ʱ� ��� ����
	for (int i = 0; i < N; i++)
	{ 
		int r, c, h, w, k;
		cin >> r >> c >> h >> w >> k;

		Knight cur{ r,c,h,w,k };
		k_arr.push_back(cur);
		init_k_arr.push_back(cur);
	}

	// �� ���
	for(int i = 0; i < Q; i++)
	{
		int id, d;
		cin >> id >> d;
		id--; // ���̵� 1����
		
		if (k_arr[id].k <= 0) continue; // �̹� ü���� 0 ���ϸ� x
		else
		{
			simulate(id, d);
		}
	}

	// �� ���
	int ans = 0;
	for (int i = 0; i < N; i++)
	{
		if (k_arr[i].k > 0)
		{
			ans += init_k_arr[i].k - k_arr[i].k; // �ʱ�ȭ ���¿� �ùķ��̼� ���� ü�� ��
		}
	}

	cout << ans << "\n";

	return 0;
}
