#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

using namespace std;

// 산타 : 1 ~ P
/*
게임판 : NxN, (r,c), 좌상단 (1,1)
게임 :
    M턴 - 루돌프 먼저 한번, 1~P까지 산타 순서대로
    기절하거나 , 격자바깥 산타는 x
거리 : (r1-r2)^2 + (c1-c2)^2

루돌프 :
    가장 가까운 산타 향해서.(격자 안, 노기절)
    가장 가까운 -> r이 큰 -> c가 큰
    상하좌우 대각선 (8방향)

산타 :
    1부터 P (격자 안)
    가까워지는 방향으로
    다른 산타가 있거나 게임판 밖은 x
    움직일 수 없거나, 가까워질 수 없으면 정지
    '상우하좌' 만 가능.

충돌 :
    루돌프 -> 산타 , 산타 += C (루돌프가 C칸 밀려남)
    산타 -> 루돌프 , 산타 += D (산타가 반대 방향으로 D칸)
    포물선 이동
    게임판 바깥이면 탈락
    다른 산타가 칸에 있으면 상호작용.

상호작용 :
    동일한 방향으로 산타 밀려남 (1칸)
    연쇄적으로 다른 산타들도 적용
기절 :
    충돌후 기절.
    k번째 턴 -> k+1 기절 -> k+2 이동
    충돌이나 상호작용 가능

종료 :
    M턴 이후 종료
    모두 탈락하면 즉시 종료
    탈락하지 않았으면 매 턴 종료마다 1점씩 추가

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
        if (in_range(s.R, s.C)) return false; // 하나라도 안에 있으면 아직 끝 아님.
    return true;
}

int get_santa_idx()
{
    int min_d = INT_MAX;
    int s_id = -1;
    pair<int, int> coor = { 0,0 };

    for (int i = 1; i <= P; i++) {
        auto s = santas[i];
        if (!in_range(s.R,s.C)) continue; // 바깥

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

    // 최적 방향 찾기
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

        // 현재 산타 이동.
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

    // 바깥 or 기절
    if (!in_range(s.R, s.C) || stuns[s_id] > 0) return;

    // 방향 구하기
    int min_d = get_dist(s.R, s.C, dear.R, dear.C); // 현재 위치
    int dir = -1;

    // 최적 방향 찾기
    for (int i = 0; i < 4; i++) {
        int nr = s.R + dirs[i][0];
        int nc = s.C + dirs[i][1];

        if (!in_range(nr, nc)) continue; // 범위 바깥이면

        bool overlapped = false;
        for (int i = 1; i <= P; i++) {
            if (i == s_id) continue;
            auto other = santas[i];
            if (other.R == nr && other.C == nc) {
                overlapped = true;
                break;
            }
        }
        if (overlapped) continue; // 다른 산타 있으면

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

    // 조기 종료확인.
    if (is_end()) return true;

    for (int i = 1; i <= P; i++) {
        if (in_range(santas[i].R, santas[i].C)) points[i]++; // 살아남은 산타 점수 추가
        stuns[i] = max(0, stuns[i] - 1); // 스턴 감소
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
