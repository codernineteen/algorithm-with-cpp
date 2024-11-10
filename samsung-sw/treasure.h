#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

int K, M;
queue<int> que;
vector<vector<int>> board(5, vector<int>(5, 0));
int dirs[4][2] = {
    {1,0},
    {-1,0},
    {0,1},
    {0, -1}
};
vector<int> ans;


// 5x5 ���� 3x3 ���� ���� ȸ�� (90/180/270)
// �����ϸ� ������ ȸ�� ����

// ���� 1�� ȹ�� ��ġ �ִ�ȭ -> ȸ�� ���� ���� �� -> �߽� ��ǥ ��, ���� ���� ��

// ���� ������ ����� �ڸ���, ť���� ������ ���.  (�� ��ȣ�� ���� �� -> �� ��ȣ�� ū ��)
//  ���Ӱ� �߰� ��, �� ���ܳ� �� ����. -> �� �̻� �ݺ����� ���� ������.

// K�� ����. (� ������ε� ȹ���� �� �������� �״�� ����.)
//  - ���� ����Ǵ� �Ͽ� �ƹ� ���� ������� ����.


bool inRange(int r, int c)
{
    return (r > -1) && (r < 5) && (c > -1) && (c < 5);
}

void rotate90(int r, int c, vector<vector<int>>& board)
{
    // copy (3x3)
    vector<vector<int>> src(3, vector<int>(3, 0));

    // rotate
    for (int i = r, sr = 0; i < r + 3; i++, sr++)
    {
        for (int j = c, sc = 0; j < c + 3; j++, sc++)
        {
            int new_r = sc;
            int new_c = 3 - sr - 1;
            src[new_r][new_c] = board[i][j];
        }
    }

    // paste
    for (int i = r, sr = 0; i < r + 3; i++, sr++)
    {
        for (int j = c, sc = 0; j < c + 3; j++, sc++)
        {
            board[i][j] = src[sr][sc];
        }
    }
}

void rotate180(int r, int c, vector<vector<int>>& board)
{
    //// width swap
    //for (int i = r; i < r + 3; i++)
    //{
    //    swap(board[i][c], board[i][c + 2]);
    //}

    //// height swap
    //for (int i = c; i < c + 3; i++)
    //{
    //    swap(board[r][i], board[r + 2][i]);
    //}
    rotate90(r, c, board);
    rotate90(r, c, board);
}

void rotate270(int r, int c, vector<vector<int>>& board)
{
    rotate90(r, c, board);
    rotate90(r, c, board);
    rotate90(r, c, board);
}

int bfs(vector<vector<int>>& board)
{
    int total = 0;
    vector<vector<int>> vis(5, vector<int>(5, 0));

    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            if (vis[i][j]) continue;

            queue<pair<int, int>> nexts;
            queue<pair<int, int>> prevs;

            vis[i][j] = 1;
            nexts.push({ i,j });
            prevs.push({ i,j });

            while (!nexts.empty())
            {
                auto p = nexts.front();
                nexts.pop();

                for (auto& dir : dirs)
                {
                    int nr = p.first + dir[0];
                    int nc = p.second + dir[1];

                    if (inRange(nr, nc) && !vis[nr][nc] && board[nr][nc] == board[p.first][p.second])
                    {
                        nexts.push({ nr, nc });
                        prevs.push({ nr, nc });
                        vis[nr][nc] = 1;
                    }
                }
            }

            if (prevs.size() >= 3)
            {
                total += prevs.size();
                while (!prevs.empty())
                {
                    auto n = prevs.front();
                    prevs.pop();

                    board[n.first][n.second] = -1;
                }
            }
        }
    }

    return total;
}

vector<vector<int>> copy_board()
{
    vector<vector<int>> tmp(5, vector<int>(5, 0));

    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < 5; c++)
        {
            tmp[r][c] = board[r][c];
        }
    }

    return tmp;
}

void fill_cell()
{
    for (int c = 0; c < 5; c++)
    {
        for (int r = 4; r > -1; r--)
        {
            if (board[r][c] == -1)
            {
                int n = que.front();
                que.pop();
                board[r][c] = n;
            }
        }
    }
}

inline bool canUpdate(int br, int bc, int bdeg, int bpoint, int r, int c, int deg, int point)
{
    if (point <= 0) return false;

    if (bpoint < point) return true;
    else if (point == bpoint)
    {
        if (deg < bdeg) return true;
        else if (deg == bdeg)
        {
            if (c < bc) return true;
            else if (c == bc)
            {
                return r < bc;
            }
            else return false;
        }
        else return false;
    }
    else return false;
}

void print_board(const vector<vector<int>>& board, const std::string& dbg_name)
{
    cout << "---------------------\n";
    cout << "[" << dbg_name << "]" << "\n";
    for (int r = 0; r < 5; r++)
    {
        for (int c = 0; c < 5; c++)
        {
            cout << board[r][c] << " ";
        }
        cout << "\n";
    }
}

bool simulate()
{
    int br = INT_MAX, bc = INT_MAX, bpoint = INT_MIN, bdeg = INT_MAX;

    // find best r, c
    cout << "Trying to find best..." << "\n";
    print_board(board, "board before");
    for (int r = 2; r > -1; r--)
    {
        for (int c = 2; c > -1; c--)
        {
            int point = 0;

            vector<vector<int>> cp_board = copy_board();
            rotate270(r, c, cp_board);
            print_board(cp_board, ("board after rotate 270(" + r + ","));
            point = bfs(cp_board);
            if (canUpdate(br, bc, bdeg, bpoint, r, c, 270, point)) { br = r; bc = c; bdeg = 270; bpoint = point; }

            cp_board = copy_board();
            rotate180(r, c, cp_board);
            print_board(cp_board, "board after rotate 180");
            point = bfs(cp_board);
            if (canUpdate(br, bc, bdeg, bpoint, r, c, 180, point)) { br = r; bc = c; bdeg = 180; bpoint = point; }

            cp_board = copy_board();
            rotate90(r, c, cp_board);
            print_board(cp_board, "board after rotate 190");
            point = bfs(cp_board);
            if (canUpdate(br, bc, bdeg, bpoint, r, c, 90, point)) { br = r; bc = c; bdeg = 90; bpoint = point; }
        }
    }

    if (bpoint == INT_MIN) return false;

    // �ʱ�ȹ����� ����ȹ�����
    int res = 0;
    // ���õ� ���� ȸ��
    cout << "Start simulation..." << "\n";
    print_board(board, "board before simulation");
    switch (bdeg)
    {
    case 90:
        rotate90(br, bc, board);
        break;
    case 180:
        rotate180(br, bc, board);
        break;
    case 270:
        rotate270(br, bc, board);
        break;
    }
    res += bfs(board);
    fill_cell();

    while (true)
    {
        print_board(board, "board before bfs");
        int p = bfs(board);
        if (p == 0) break;

        fill_cell();
        print_board(board, "board after bfs");
        res += p;
    }

    cout << "\n";

    ans.push_back(res);

    return true;
}


int solve() {
    // ���⿡ �ڵ带 �ۼ����ּ���.
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> K >> M;
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            cin >> board[i][j];
        }
    }


    while (M-- > 0)
    {
        int t;
        cin >> t;
        que.push(t);
    }

    while (K-- > 0)
    {
        bool notEnd = simulate();
        if (!notEnd) break;
    }

    for (auto& n : ans)
    {
        cout << n << " ";
    }

    return 0;
}
