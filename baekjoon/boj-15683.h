#pragma once

#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>
#include <climits>

using namespace std;

int N, M; // 1 ~ 8
int board[9][9] = { 0, };

int dirs[4][2] = { {-1,0}/*╩С*/, {0,1} /*©Л*/, {1,0}/*го*/, {0,-1}/*аб*/};
vector<vector<int>> cctvs; // {type, r, c}
vector<int> ans; 

int cctv_cnt = 0;
int wall_cnt = 0;
int cell_cnt = 0;

bool isInRange(int nr, int nc)
{
	return (nr < N && nc < M && nr > -1 && nc > -1);
}

void backtrack(int idx, int score)
{
	if (idx >= cctvs.size())
	{
		ans.push_back(cell_cnt - wall_cnt - cctv_cnt - score);
		return;
	}


	int t = cctvs[idx][0];
	int r = cctvs[idx][1];
	int c = cctvs[idx][2];
	queue<pair<int, int>> traces;

	if (t == 1)
	{
		for (int i = 0; i < 4; i++)
		{
			int nr = r + dirs[i][0];
			int nc = c + dirs[i][1];
			int removed = 0;

			while (isInRange(nr, nc) && board[nr][nc] != 6)
			{
				if (board[nr][nc] == 0)
				{
					board[nr][nc] = 7;
					traces.push({ nr, nc });
					removed++;
				}

				nr += dirs[i][0];
				nc += dirs[i][1];
			}
			//print_board();
			// jump next
			backtrack(idx + 1, score + removed);

			// backtrack (rollback cell)
			while (!traces.empty())
			{
				auto p = traces.front(); 
				traces.pop();
				board[p.first][p.second] = 0;
			}
		}
	}
	else if (t == 2)
	{
		for (int i = 0; i < 2; i++)
		{
			int removed = 0;
			vector<int> target_dir = { i, i + 2 };

			for (auto dir : target_dir)
			{
				int nr = r + dirs[dir][0];
				int nc = c + dirs[dir][1];

				while (isInRange(nr, nc) && board[nr][nc] != 6)
				{
					
					if (board[nr][nc] == 0)
					{
						board[nr][nc] = 7;
						traces.push({ nr, nc });
						removed++;
					}

					nr += dirs[dir][0];
					nc += dirs[dir][1];
				}
			}
			
			// jump next
			backtrack(idx + 1, score + removed);

			// backtrack (rollback cell)
			while (!traces.empty())
			{
				auto p = traces.front();
				traces.pop();
				board[p.first][p.second] = 0;
			}
		}
	}
	else if (t == 3)
	{
		for (int i = 0; i < 4; i++)
		{
			int removed = 0;
			vector<int> target_dir = { i, (i + 1) % 4 };

			for (auto dir : target_dir)
			{
				int nr = r + dirs[dir][0];
				int nc = c + dirs[dir][1];

				while (isInRange(nr, nc) && board[nr][nc] != 6)
				{
					if (board[nr][nc] == 0)
					{
						board[nr][nc] = 7;
						traces.push({ nr, nc });
						removed++;
					}

					nr += dirs[dir][0];
					nc += dirs[dir][1];
				}
			}

			// jump next
			backtrack(idx + 1, score + removed);

			// backtrack (rollback cell)
			while (!traces.empty())
			{
				auto p = traces.front();
				traces.pop();
				board[p.first][p.second] = 0;
			}
		}
	}
	else if (t == 4)
	{
		for (int i = 0; i < 4; i++)
		{
			int removed = 0;
			vector<int> target_dir = { (i - 1 + 4) % 4, (i + 4) % 4, (i + 1 + 4) % 4 };

			for (auto dir : target_dir)
			{
				int nr = r + dirs[dir][0];
				int nc = c + dirs[dir][1];

				while (isInRange(nr, nc) && board[nr][nc] != 6)
				{
					if (board[nr][nc] == 0)
					{
						board[nr][nc] = 7;
						traces.push({ nr, nc });
						removed++;

					}
					nr += dirs[dir][0];
					nc += dirs[dir][1];
				}
			}

			// jump next
			backtrack(idx + 1, score + removed);

			// backtrack (rollback cell)
			while (!traces.empty())
			{
				auto p = traces.front();
				traces.pop();
				board[p.first][p.second] = 0;
			}
		}
	}
	else
	{
		int removed = 0;

		for (int dir = 0; dir < 4; dir++)
		{
			int nr = r + dirs[dir][0];
			int nc = c + dirs[dir][1];

			while (isInRange(nr, nc) && board[nr][nc] != 6)
			{
				if (board[nr][nc] == 0)
				{
					board[nr][nc] = 7;
					traces.push({ nr, nc });
					removed++;
				}

				nr += dirs[dir][0];
				nc += dirs[dir][1];
			}
		}

		// jump next
		backtrack(idx + 1, score + removed);

		// backtrack (rollback cell)
		while (!traces.empty())
		{
			auto p = traces.front();
			traces.pop();
			board[p.first][p.second] = 0;
		}
	}
	
}

int solve()
{
	ios::sync_with_stdio(0);
	cin.tie(0);



	cin >> N >> M;
	cell_cnt = N * M;

	for (int i = 0; i < N; i++)
	{
		for (int j = 0; j < M; j++)
		{
			cin >> board[i][j];
			int c = board[i][j];
			if (board[i][j] > 0 && board[i][j] < 6)
			{
				//cctv
				cctvs.push_back({ c, i, j });
			}
			else if (c == 6)
			{
				wall_cnt++;
			}
		}
	}
	cctv_cnt = cctvs.size();

	backtrack(0, 0);

	//print ans;
	int res = INT_MAX;
	for (auto& n : ans) res = min(res, n);

	cout << res << "\n";

	return 0;
}
