#include  "types.h"

using namespace std;

// n x n ����(n�� Ȧ��) , ������ ���߾ӿ��� ���� (n/2, n/2) -> ¦����?
// m���� ������, �߾ӿ� ���� ��ġ�� �־���.
// ������ �����ڰ� ������ �־����� ���� �����ϴ�.
/*
    �����ڴ� ���� Ȥ�� �¿�θ� �����δ�.
    - �¿� �����ڴ� �׻� ������ ���� ���� (0,1)
    - ���� �����ڴ� �׻� �Ʒ��� ���� ���� (1,0)
    - �̵� ���� �������� ��ġ�� ��ĥ �� ����.

    1. m���� �����ڰ� ���� '���ÿ�' �����δ�.
        - ���� �������� �Ÿ��� 3 ������ �����ڸ� �����δ�. (����ư �Ÿ�)
        - ���ڸ� ����� ���
            ������ �ݴ�� ư��
            ������ ���ٸ� 1ĭ �̵�
        - ����� �ʴ� ���
            �����̷��� ĭ�� ������ ������ �������� �ʴ´�.
            ������ ���� �ʴٸ�, �ش� ĭ���� �����δ�.
    2. ������ �����δ�
        - ó�� �� �������� ���� �����Ͽ� ������ ������� ������
        - ���� �����ϸ� �ٽ� �߽����� �̵�
        - ������ ��Ȯ�� �� ĭ�� �������� ������
            �̵������� Ʋ������ ��ġ��� ������ �ٷ� �ٲ���´�.(���߾�, �簢����, (1,1))
        - '�̵� ����', ������ �þ� ���� �����ڸ� ��´�,
            - ���� �ٶ󺸴� ���� ���� �þ� 3ĭ (���� ũ�⿡ �������)
                - �̵������� Ʋ������ ������ �� ����ϱ�
            - ������ ������ ĭ�� ������ �ʴ´�.
        - ������ ���� �� t x ���� ������ �� ��ŭ ������ ����.
        - ���� �����ڴ� �������.
*/

struct Runner {
    int row;
    int col;
    int dir; // 1 : �¿�, 2 : ����
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

int n, m, h, k; // m : ������ , h : ���� ����, k ��
int dirs[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1} }; // �������
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
    if (dist > 3) return; // �Ÿ� 3 �ʰ� -> ����

    // ���� �Ǵ�
    int nr = curR.row + dirs[curR.dir][0];
    int nc = curR.col + dirs[curR.dir][1];
    if (nr > n || nc > n || nr < 1 || nc < 1) {
        //���� ����� ���
        // 1. ���� ��ȯ
        curR.dir = (curR.dir + 2) % 4;
        // 2. ���� �༱�� �ٽ� ���
        nr = curR.row + dirs[curR.dir][0];
        nc = curR.col + dirs[curR.dir][1];
        // 3. ���� �ִ��� �Ǵ� �� �̵�
    }

    // ���� ���� ���� �ٷ� �̵�.
    if (nr == tracer.row && nc == tracer.col) return; // ������ ������ ����

    // �̵�
    curR.row = nr;
    curR.col = nc;
}

int grab() {
    // ���
    int take = 0;
    for (int i = 0; i < 3; i++) {
        int nr = tracer.row + dirs[tracer.dir][0] * i;
        int nc = tracer.col + dirs[tracer.dir][1] * i;

        if (nr > n || nc > n || nr < 1 || nc < 1) continue; // ��谡 ����� ���� ���� ����

        for (int i = 1; i <= m; i++) {
            if (!runner[i].alive) continue;

            // ������ �����������ʰ�, ��ġ�� ������
            if (board[nr][nc] != 1 && runner[i].row == nr && runner[i].col == nc) { // ����
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
        tracer.dir = 2; // ���ݴ�
        tracer.dist = n - 1;
        tracer.walk = 0;
        tracer.count = 3;
        isForward = false;
    }
    else {
        if (tracer.walk == tracer.dist) {
            tracer.dir = (tracer.dir + 1) % 4; // ���� �Ÿ��� ���� ���� ���̿� ���� �� ������ 90�� ȸ��
            tracer.count--; // ī��Ʈ ����

            if (tracer.count > 0) { // dist ����
                tracer.walk = 0; // ���� �Ÿ� �ʱ�ȭ
            }
            else { // �Ÿ� Ȯ��
                tracer.walk = 0; // ���� �Ÿ� �ʱ�ȭ
                if (tracer.dist == n - 2) tracer.count = 3; // ������ �����̸� 3���� ����
                else tracer.count = 2;
                tracer.dist++; // �Ÿ� Ȯ��
            }
        }
    }

    // ��� �õ�
    int take = grab();
    // ���� ������Ʈ
    tracer.score += (take * turn);
}

void backwardTrace(int turn, bool& isForward) {
    tracer.row = tracer.row + dirs[tracer.dir][0];
    tracer.col = tracer.col + dirs[tracer.dir][1];
    tracer.walk++;


    if (tracer.row == n / 2 + 1 && tracer.col == n / 2 + 1) {
        tracer.row = n / 2 + 1;
        tracer.row = n / 2 + 1;
        tracer.dir = 0; // ���ݴ�
        tracer.dist = 1;
        tracer.walk = 0;
        tracer.count = 2;
        isForward = true;
    }
    else {
        if (tracer.walk == tracer.dist) {
            tracer.dir--;
            if (tracer.dir < 0) tracer.dir = 3; // ������ ��ȯ

            tracer.count--; // ī��Ʈ ����
            if (tracer.count > 0) { // dist ����
                tracer.walk = 0; // ���� �Ÿ� �ʱ�ȭ
            }
            else { // �Ÿ� Ȯ��
                tracer.walk = 0; // ���� �Ÿ� �ʱ�ȭ
                tracer.count = 2; // ������ ���� ó�� ���� �׻� 2
                tracer.dist--; // �Ÿ� Ȯ��
            }
        }
    }

    // ��� �õ�
    int take = grab();
    // ���� ������Ʈ
    tracer.score += (take * turn);
}

void simulate(int turn, bool& isForward) {
    for (int i = 1; i <= m; i++) {
        // �����ڰ� ������ �ʾ��� ����
        if (runner[i].alive) runaway(i);
    }

    // ���� �ൿ
    if (isForward) forwardTrace(turn, isForward);
    else backwardTrace(turn, isForward);
}

int solve() {
    // ���⿡ �ڵ带 �ۼ����ּ���.
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> n >> m >> h >> k;
    remain = m;
    // ���߾� ��ġ
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
