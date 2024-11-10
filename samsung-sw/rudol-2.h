#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

using namespace std;

// ��Ÿ : 1 ~ P
/*
������ : NxN, (r,c), �»�� (1,1)
���� :
    M�� - �絹�� ���� �ѹ�, 1~P���� ��Ÿ �������
    �����ϰų� , ���ڹٱ� ��Ÿ�� x
�Ÿ� : (r1-r2)^2 + (c1-c2)^2

�絹�� :
    ���� ����� ��Ÿ ���ؼ�.(���� ��, �����)
    ���� ����� -> r�� ū -> c�� ū
    �����¿� �밢�� (8����)

��Ÿ :
    1���� P (���� ��)
    ��������� ��������
    �ٸ� ��Ÿ�� �ְų� ������ ���� x
    ������ �� ���ų�, ������� �� ������ ����
    '�������' �� ����.

�浹 :
    �絹�� -> ��Ÿ , ��Ÿ += C (�絹���� Cĭ �з���)
    ��Ÿ -> �絹�� , ��Ÿ += D (��Ÿ�� �ݴ� �������� Dĭ)
    ������ �̵�
    ������ �ٱ��̸� Ż��
    �ٸ� ��Ÿ�� ĭ�� ������ ��ȣ�ۿ�.

��ȣ�ۿ� :
    ������ �������� ��Ÿ �з��� (1ĭ)
    ���������� �ٸ� ��Ÿ�鵵 ����
���� :
    �浹�� ����.
    k��° �� -> k+1 ���� -> k+2 �̵�
    �浹�̳� ��ȣ�ۿ� ����

���� :
    M�� ���� ����
    ��� Ż���ϸ� ��� ����
    Ż������ �ʾ����� �� �� ���Ḷ�� 1���� �߰�

*/
#define R first
#define C second
#define MAX_S 31

int N, M, P, Cp, D;
pair<int, int> dear;

int points[MAX_S] = {0,};
int stuns[MAX_S] = { 0, };
int dirs[4][2] = { {-1,0}, {0, 1}, {1,0},{0,-1} };
int r_dirs[8][2] = { {-1,0}, {0, 1}, {1,0},{0,-1}, {1,1}, {-1,-1}, {-1,1}, {1,-1} };

pair<int, int> santas[MAX_S] = { {0,0}, };

int get_dist(int r1, int c1, int r2, int c2) {
    int r_d = r1 - r2;
    int c_d = c1 - c2;
    return r_d * r_d + c_d * c_d;
}

int in_range(int r, int c) {
    return (1 <= r && r <= N && 1 <= c && c <= N);
}

bool is_end()
{
    for (auto& s : santas)
        if (in_range(s.R, s.C)) return false; // �ϳ��� �ȿ� ������ ���� �� �ƴ�.
    return true;
}

int get_santa_idx()
{
    int min_d = INT_MAX;
    int s_id = -1;
    pair<int, int> coor = { 0,0 };

    for (int i = 1; i <= P; i++) {
        auto s = santas[i];
        if (!in_range(s.R,s.C)) continue; // �ٱ�

        int cur_d = get_dist(dear.R, dear.C, s.R, s.C);
        if (cur_d < min_d) {
            min_d = cur_d;
            s_id = i;
            coor = s;
        }
        else if (cur_d == min_d) {
            if (s.R > coor.R) {
                min_d = cur_d;
                s_id = i;
                coor = s;
            }
            else if (s.R == coor.R) {
                if (s.C > coor.C) {
                    min_d = cur_d;
                    s_id = i;
                    coor = s;
                }
            }
        }
    }

    return s_id;
}

int get_min_dir(int s_id)
{
    auto s = santas[s_id];
    int min_d = get_dist(s.R, s.C, dear.R, dear.C);
    int dir = -1;

    // ���� ���� ã��
    for (int i = 0; i < 8; i++) {
        int nr = dear.R + r_dirs[i][0];
        int nc = dear.C + r_dirs[i][1];

        if (!in_range(nr, nc)) continue;

        int dist = get_dist(s.R, s.C, nr, nc);
        if (dist < min_d) {
            min_d = dist;
            dir = i;
        }
    }

    return dir;
}

void crush(int s_id, int turn, int r_delta, int c_delta, int power)
{   
    int cur_id = s_id;
    bool overlapped = true;

    bool init = true;

    while (overlapped) {
        auto& cur_s = santas[cur_id];
        overlapped = false;
        
        int nr = cur_s.R + (init ? r_delta * power : r_delta);
        int nc = cur_s.C + (init ? c_delta * power : c_delta);
        init = false;

        for (int i = 1; i <= P; i++) {
            if (i == cur_id) continue;

            auto other = santas[i];
            if (other.R == nr && other.C == nc) {
                cur_id = i;
                overlapped = true;

                break;
            }
        }

        // ���� ��Ÿ �̵�.
        cur_s = { nr, nc };
    }
}

void dear_act(int turn)
{
    int s_id = get_santa_idx();
    int d_id = get_min_dir(s_id);

    int nr = dear.R + r_dirs[d_id][0];
    int nc = dear.C + r_dirs[d_id][1];
    auto s = santas[s_id];

    // move
    dear = { nr, nc };

    // check overlap
    if (dear.R == s.R && dear.C == s.C) {
        // crush
        crush(s_id, turn, r_dirs[d_id][0], r_dirs[d_id][1], Cp);
        points[s_id] += Cp;
        stuns[s_id] = 2;
    }
}

void santa_act(int turn, int s_id)
{
    auto& s = santas[s_id];

    // �ٱ� or ����
    if (!in_range(s.R, s.C) || stuns[s_id] > 0) return;

    // ���� ���ϱ�
    int min_d = get_dist(s.R, s.C, dear.R, dear.C); // ���� ��ġ
    int dir = -1;

    // ���� ���� ã��
    for (int i = 0; i < 4; i++) {
        int nr = s.R + dirs[i][0];
        int nc = s.C + dirs[i][1];

        if (!in_range(nr, nc)) continue; // ���� �ٱ��̸�

        bool overlapped = false;
        for (int i = 1; i <= P; i++) {
            if (i == s_id) continue;
            auto other = santas[i];
            if (other.R == nr && other.C == nc) {
                overlapped = true;
                break;
            }
        }
        if (overlapped) continue; // �ٸ� ��Ÿ ������

        int dist = get_dist(dear.R, dear.C, nr, nc);
        if (dist < min_d) {
            min_d = dist;
            dir = i;
        }
    }

    if (dir == -1) return;

    int nr = s.R + dirs[dir][0];
    int nc = s.C + dirs[dir][1];

    santas[s_id] = { nr, nc };

    // check overlap
    if (dear.R == nr && dear.C == nc) {
        // crush
        crush(s_id, turn, -dirs[dir][0], -dirs[dir][1], D);
        points[s_id] += D;
        stuns[s_id] = 2;
    }
}

bool simulate(int turn)
{
    dear_act(turn);

    for (int i = 1; i <= P; i++) {
        santa_act(turn, i);
    }

    // ���� ����Ȯ��.
    if (is_end()) return true;

    for (int i = 1; i <= P; i++) {
        if (in_range(santas[i].R, santas[i].C)) points[i]++; // ��Ƴ��� ��Ÿ ���� �߰�
        stuns[i] = max(0, stuns[i] - 1); // ���� ����
    }

    return false;
}

int solve()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> N >> M >> P >> Cp >> D;

    int dr, dc;
    cin >> dr >> dc;
    dear = { dr, dc };
    
    for (int i = 0; i < P; i++) {
        int id, r, c;
        cin >> id >> r >> c;
        santas[id] = { r, c };
    }
   
    int turn = M;
    while (M-- > 0) {
        simulate(turn);
    }

    for (int i = 1; i <= P; i++)
        cout << points[i] << " ";
    cout << "\n";

    return 0;
}
