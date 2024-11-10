#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;
#define pii pair<int, int>
#define R first
#define C second
#define NO 0
#define E 1
#define S 2
#define W 3

extern class Golem;
int isExit[75][75] = {0,};
int board[75][75] = { 0, };

// ºÏ µ¿ ³² ¼­
int dirs[4][2] = {
    {-1,0},
    {0,1},
    {1,0},
    {0,-1}
};

int M, N, K;
int ans;

bool inRange(int r, int c)
{
    return (r >= 3 && r < M+3 && c >= 1 && c <= N);
}

class Golem {
public:
    Golem(int c, int d, int id) { center = { 1, c }; exit = d; this->id = id;  };
    ~Golem() { }

    void clear()
    {
        board[center.R][center.C] = 0;
        for (auto& dir : dirs)
        {
            int r = center.R + dir[0];
            int c = center.C + dir[1];
            board[r][c] = 0;
        }
    }


    void move(int d)
    {
        // clear
        clear();

        // move
        center.R += dirs[d][0];
        center.C += dirs[d][1];

        // fill
        board[center.R][center.C] = id;
        for (int i = 0; i < 4; i++)
        {
            int nr = center.R + dirs[i][0];
            int nc = center.C + dirs[i][1];

            board[nr][nc] = id;
        }
    }

    bool move_south()
    {
        int nr = center.R + 1 + dirs[S][0];
        int nc = center.C + dirs[S][1];

        if (nr < M + 3 && board[nr][nc] == 0 && board[nr-1][nc-1] == 0 && board[nr-1][nc + 1] == 0) { move(S); return true; }

        return false;
    }

    bool move_west()
    {
        int nr = center.R + dirs[W][0];
        int nc = center.C - 1 + dirs[W][1];

        if (nc >= 1 && board[nr][nc] == 0 && board[nr + 1][nc + 1] == 0 && board[nr - 1][nc + 1] == 0) {
            move(W);
            if (move_south()) { 
                exit = (exit + 3) % 4;
                return true; 
            }
            else move(E);
        }

        return false;
    }

    bool move_east()
    {
        int nr = center.R + dirs[E][0];
        int nc = center.C + 1 + dirs[E][1];

        if (nc <= N && board[nr][nc] == 0 && board[nr + 1][nc-1] == 0 && board[nr - 1][nc-1] == 0) {
            move(E);
            if (move_south()) {
                exit = (exit + 1) % 4;
                return true;
            }
            else move(W);
        }

        return false;
    }

    bool uni_step()
    {
        if (move_south()) return true;
        else if (move_west()) return true;
        else if (move_east()) return true;
        else return false;
    }

    bool is_body_in()
    {
        if (!inRange(center.R, center.C)) return false;
        for (auto& dir : dirs)
        {
            int r = center.R + dir[0];
            int c = center.C + dir[1];
            if (!inRange(r, c)) return false;
        }

        return true;
    }

    int bfs()
    {
        queue<pair<int, int>> nexts;
        nexts.push({ center.first, center.second });

        vector<vector<int>> vis(M+4, vector<int>(N+4, 0));
        vis[center.R][center.C] = 1;

        int max_row = center.R;

        while (!nexts.empty())
        {
            auto cur = nexts.front(); nexts.pop();

            for (auto& dir : dirs)
            {
                int nr = cur.R + dir[0];
                int nc = cur.C + dir[1];

                if (inRange(nr, nc) && !vis[nr][nc] && (board[cur.R][cur.C] == board[nr][nc] || (isExit[cur.R][cur.C] && board[nr][nc] != 0)))
                {
                    nexts.push({ nr, nc });
                    max_row = max(max_row, nr);
                    vis[nr][nc] = 1;
                }
            }
        }

        return max_row - 2;
    }

    void simulate()
    {
        while (uni_step()) {}
        isExit[center.R + dirs[exit][0]][center.C + dirs[exit][1]] = 1;

        if (!is_body_in()) {
            for (int i = 0; i <= M+3; i++)
            {
                for (int j = 0; j <= N; j++)
                {
                    board[i][j] = 0;
                    isExit[i][j] = 0;
                }
            }
            return;
        }

        // calc score
        int p = bfs();
        ans += p;
    }



    pii center;
    int exit;
    int id;
};

int solve()
{
    cin >> M >> N >> K;

    int id = 1;
    while (K--)
    {
        int c, d;
        cin >> c >> d;

        Golem* g = new Golem(c, d, id);

        g->simulate();
        id+=1;
    }

    cout << ans << "\n";


    return 0;
}
