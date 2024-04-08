#include "types.h"

// n x n 격자
// 각각의 셀에는 무기가 '있을' 수도 잇음
// 초기 플레이어들은 무기가 '없는' 셀에 위치 + 초기 능력치
/*
1. 첫번째 플레이어부터 '순차적'으로 향하는 방향으로 한 칸이동. 격자를 벗어나면 방향을 정반대로 바꿔서 '1이동'
2. 이동한 방향에 플레이어가 없으면, 총이 있는지 확인.
    총이 있다면
        - 이미 가지고 있었던 경우, 공격력이 더 쎈 총을 획득하고 나머지는 격자에 둠
        - 없는 경우, 바로 획득
    플레이어가 있다면 -> 싸움
        초기 능력치 + 가진 총 공격력 합이 큰 사람
        -> 같으면, 초기 능력치가 큰 사람
    이긴 사람은 |(p1능력치 + p1 총 공격력) - (p2능력치 + p2 총 공격력)| 만큼 포인트 획득
    진 사람은 가지고 있던 총을 격자에 놓고, 방향대로 이동
        범위 밖이거나 , 칸에 다른 사람이 있으면 오른쪽으로 90도 회전 후 빈 칸이 보일 때 이동.
        마찬가지로 이동 후 총 획득 및 나머지 총 격자에 둠
*/
using namespace std;

struct Player {
    int row = 0;
    int col = 0;
    int dir = 0;
    int stat = 0;
    int gun = 0;
   // bool isMoved = false;
};


int n, m, k;
int score[31] = { 0, };
int dirs[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1} }; // 상우하좌
Player players[31];
vector<vector<vector<int>>> board(21, vector<vector<int>>(21, vector<int>()));

bool movePlayer(int pId) {
    auto& p = players[pId];

    int nr = p.row + dirs[p.dir][0];
    int nc = p.col + dirs[p.dir][1];

    if (nr > n || nc > n || nr < 1 || nc < 1) { // 격자 벗어남
        p.dir = (p.dir + 2) % 4; // 반대 전환
        // 다시 1칸 이동
        nr = p.row + dirs[p.dir][0];
        nc = p.col + dirs[p.dir][1];
    }
    // 이동
    p.row = nr;
    p.col = nc;
    return true;
}

void pickGun(int pId) {
    auto& curP = players[pId];
    if (!board[curP.row][curP.col].empty()) {
        //총이 있으면,
        int maxGun = 0;
        int maxId = 0;
        auto& guns = board[curP.row][curP.col];
        // 최대 공격력 총 찾기.
        for (int gId = 0; gId < guns.size(); gId++) {
            if (guns[gId] > maxGun) {
                maxGun = guns[gId];
                maxId = gId;
            }
        }

        // 플레이어가 가진 것보다 쎄면 총 제거
        if (maxGun > curP.gun) {
            guns.erase(guns.begin() + maxId);
            if (curP.gun != 0) { // 플레이어가 총을 가지고 있었으면
                guns.push_back(curP.gun); // 가지고 있던 총을 삽입
            }
            curP.gun = maxGun; // 플레이어 총 업데이트
        }
    }
}

void fight(int pId) {
    auto& curP = players[pId];
    int tId = -1;

    for (int i = 1; i <= m; i++) {
        auto& other = players[i];
        if (i != pId && other.row == curP.row && other.col == curP.col) {
            tId = i;
            break;
        }
    }

    if (tId != -1) {
        // 싸움
        int win = -1;
        int lose = -1;

        auto& other = players[tId];

        int sum1 = curP.stat + curP.gun;
        int sum2 = other.stat + other.gun;

        if (sum1 > sum2) {
            //내가 이김
            win = pId;
            lose = tId;
        }
        else if (sum1 == sum2) {
            if (curP.stat > other.stat) { // 내가 이김
                win = pId;
                lose = tId;
            }
            else { // other이김
                win = tId;
                lose = pId;
            }
        }
        else {
            // other이김
            win = tId;
            lose = pId;
        }

        // 점수 업데이트
        score[win] += abs(sum1 - sum2);

        // 진 사람 행동 먼저
        auto& loser = players[lose];
        if (loser.gun > 0) { //총을 가지고 있으면, 내려 놓기
            board[loser.row][loser.col].push_back(loser.gun);
            loser.gun = 0;
        }

        while (true) {
            int nr = loser.row + dirs[loser.dir][0];
            int nc = loser.col + dirs[loser.dir][1];

            if (nr > n || nc > n || nr < 1 || nc < 1) {
                loser.dir = (loser.dir + 1) % 4; // 오른쪽 90도 회전
                continue;
            }
            bool isExist = false;
            for (int i = 1; i <= m; i++) {
                if (players[i].row == nr && players[i].col == nc) {
                    isExist = true;
                    break;
                }
            }
            if (isExist) { // 가려는 곳에 사람이 있다면
                loser.dir = (loser.dir + 1) % 4; // 오른쪽 90도 회전
                continue;
            }

            // 움직이고 break
            loser.row = nr;
            loser.col = nc;
            break;
        }
        pickGun(lose);

        // 승자 행동
        pickGun(win);
    }
    else {
        // 총 고르기
        pickGun(pId);
    }
}

void simulate() {
    for (int i = 1; i <= m; i++) {
        movePlayer(i);
        fight(i);
    }
}


int solve() {
    // 여기에 코드를 작성해주세요.
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m >> k;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= n; j++) {
            int gun;
            cin >> gun;
            if (gun != 0) board[i][j].push_back(gun);
        }
    }

    for (int i = 1; i <= m; i++) {
        int x, y, d, s;
        cin >> x >> y >> d >> s;
        players[i] = Player{ x, y, d, s, 0 };
    }

    while (k-- > 0)
    {
        simulate();
    }

    for (int i = 1; i <= m; i++)
        cout << score[i] << " ";

    return 0;
}
