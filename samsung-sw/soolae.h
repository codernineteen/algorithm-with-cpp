#include  "types.h"

using namespace std;

// n x n 격자(n은 홀수) , 술래는 정중앙에서 시작 (n/2, n/2) -> 짝수면?
// m명의 도망자, 중앙에 없고 위치는 주어짐.
// 나무와 도망자가 겹쳐져 주어지는 것이 가능하다.
/*
    도망자는 상하 혹은 좌우로만 움직인다.
    - 좌우 도망자는 항상 오른쪽 보고 시작 (0,1)
    - 상하 도망자는 항상 아래쪽 보고 시작 (1,0)
    - 이동 도중 도망자의 위치는 겹칠 수 있음.

    1. m명의 도망자가 먼저 '동시에' 움직인다.
        - 현재 술래와의 거리가 3 이하인 도망자만 움직인다. (멘헤튼 거리)
        - 격자를 벗어나는 경우
            방향을 반대로 튼다
            술래가 없다면 1칸 이동
        - 벗어나지 않는 경우
            움직이려는 칸에 술래가 있으면 움직이지 않는다.
            술래가 있지 않다면, 해당 칸으로 움직인다.
    2. 술래가 움직인다
        - 처음 위 방향으로 보고 시작하여 달팽이 모양으로 움직임
        - 끝에 도달하면 다시 중심으로 이동
        - 술래는 정확히 한 칸만 방향으로 움직임
            이동방향이 틀어지는 위치라면 방향을 바로 바꿔놓는다.(정중앙, 사각지대, (1,1))
        - '이동 직후', 술래는 시야 내에 도망자를 잡는다,
            - 현재 바라보는 방향 기준 시야 3칸 (격자 크기에 상관없음)
                - 이동방향이 틀어지는 지점을 잘 고려하기
            - 나무에 가려진 칸은 잡히지 않는다.
        - 술래는 현재 턴 t x 잡은 도망자 수 만큼 점수를 얻음.
        - 잡힌 도망자는 사라진다.
*/

struct Runner {
    int row;
    int col;
    int dir; // 1 : 좌우, 2 : 상하
    bool alive;
};

struct Tracer {
    int row = 0;
    int col = 0;
    int dist = 1;
    int count = 2;
    int walk = 0;
    int dir = 0;
    int score = 0;
};

int n, m, h, k; // m : 도망자 , h : 나무 개수, k 턴
int dirs[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1} }; // 상우하좌
int board[100][100] = { {0,} };
int remain = 0;
Runner runner[9902];
Tracer tracer{};

inline int getDist(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

void runaway(int rid) {
    auto& curR = runner[rid];
    int dist = getDist(curR.row, curR.col, tracer.row, tracer.col);
    if (dist > 3) return; // 거리 3 초과 -> 리턴

    // 격자 판단
    int nr = curR.row + dirs[curR.dir][0];
    int nc = curR.col + dirs[curR.dir][1];
    if (nr > n || nc > n || nr < 1 || nc < 1) {
        //격자 벗어나는 경우
        // 1. 방향 전환
        curR.dir = (curR.dir + 2) % 4;
        // 2. 다음 행선지 다시 계산
        nr = curR.row + dirs[curR.dir][0];
        nc = curR.col + dirs[curR.dir][1];
        // 3. 술래 있는지 판단 후 이동
    }

    // 격자 안인 경우는 바로 이동.
    if (nr == tracer.row && nc == tracer.col) return; // 술래가 있으면 멈춤

    // 이동
    curR.row = nr;
    curR.col = nc;
}

int grab() {
    // 잡기
    int take = 0;
    for (int i = 0; i < 3; i++) {
        int nr = tracer.row + dirs[tracer.dir][0] * i;
        int nc = tracer.col + dirs[tracer.dir][1] * i;

        if (nr > n || nc > n || nr < 1 || nc < 1) continue; // 경계가 벗어나는 곳은 보지 않음

        for (int i = 1; i <= m; i++) {
            if (!runner[i].alive) continue;

            // 나무에 가려져있지않고, 위치가 맞을때
            if (board[nr][nc] != 1 && runner[i].row == nr && runner[i].col == nc) { // 잡음
                runner[i].alive = false;
                take++;
                remain--;
            }
        }
    }

    return take;
}

void forwardTrace(int turn, bool& isForward) {
    tracer.row = tracer.row + dirs[tracer.dir][0];
    tracer.col = tracer.col + dirs[tracer.dir][1];
    tracer.walk++;

    if (tracer.row == 1 && tracer.col == 1) {
        tracer.row = 1;
        tracer.row = 1;
        tracer.dir = 2; // 정반대
        tracer.dist = n - 1;
        tracer.walk = 0;
        tracer.count = 3;
        isForward = false;
    }
    else {
        if (tracer.walk == tracer.dist) {
            tracer.dir = (tracer.dir + 1) % 4; // 걸은 거리가 현재 변의 길이와 같을 때 오른쪽 90도 회전
            tracer.count--; // 카운트 감소

            if (tracer.count > 0) { // dist 유지
                tracer.walk = 0; // 걸은 거리 초기화
            }
            else { // 거리 확장
                tracer.walk = 0; // 걸은 거리 초기화
                if (tracer.dist == n - 2) tracer.count = 3; // 다음이 최종이면 3으로 충전
                else tracer.count = 2;
                tracer.dist++; // 거리 확장
            }
        }
    }

    // 잡기 시도
    int take = grab();
    // 점수 업데이트
    tracer.score += (take * turn);
}

void backwardTrace(int turn, bool& isForward) {
    tracer.row = tracer.row + dirs[tracer.dir][0];
    tracer.col = tracer.col + dirs[tracer.dir][1];
    tracer.walk++;


    if (tracer.row == n / 2 + 1 && tracer.col == n / 2 + 1) {
        tracer.row = n / 2 + 1;
        tracer.row = n / 2 + 1;
        tracer.dir = 0; // 정반대
        tracer.dist = 1;
        tracer.walk = 0;
        tracer.count = 2;
        isForward = true;
    }
    else {
        if (tracer.walk == tracer.dist) {
            tracer.dir--;
            if (tracer.dir < 0) tracer.dir = 3; // 역방향 전환

            tracer.count--; // 카운트 감소
            if (tracer.count > 0) { // dist 유지
                tracer.walk = 0; // 걸은 거리 초기화
            }
            else { // 거리 확장
                tracer.walk = 0; // 걸은 거리 초기화
                tracer.count = 2; // 역으로 갈땐 처음 빼고 항상 2
                tracer.dist--; // 거리 확장
            }
        }
    }

    // 잡기 시도
    int take = grab();
    // 점수 업데이트
    tracer.score += (take * turn);
}

void simulate(int turn, bool& isForward) {
    for (int i = 1; i <= m; i++) {
        // 도망자가 잡히지 않았을 때만
        if (runner[i].alive) runaway(i);
    }

    // 술래 행동
    if (isForward) forwardTrace(turn, isForward);
    else backwardTrace(turn, isForward);
}

int solve() {
    // 여기에 코드를 작성해주세요.
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m >> h >> k;
    remain = m;
    // 정중앙 배치
    tracer.row = n / 2 + 1;
    tracer.col = n / 2 + 1;

    for (int i = 1; i <= m; i++) {
        int x, y, d;
        cin >> x >> y >> d;
        runner[i] = Runner{ x,y,d,true };
    }

    for (int i = 1; i <= h; i++) {
        int r, c;
        cin >> r >> c;
        board[r][c] = 1;
    }

    int turn = 1;
    bool isForward = true;
    while (k-- > 0 && remain > 0) {
        simulate(turn, isForward);
        turn++;
    }

    cout << tracer.score << "\n";

    return 0;
}
