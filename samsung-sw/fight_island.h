#include "types.h"

// n x n ����
// ������ ������ ���Ⱑ '����' ���� ����
// �ʱ� �÷��̾���� ���Ⱑ '����' ���� ��ġ + �ʱ� �ɷ�ġ
/*
1. ù��° �÷��̾���� '������'���� ���ϴ� �������� �� ĭ�̵�. ���ڸ� ����� ������ ���ݴ�� �ٲ㼭 '1�̵�'
2. �̵��� ���⿡ �÷��̾ ������, ���� �ִ��� Ȯ��.
    ���� �ִٸ�
        - �̹� ������ �־��� ���, ���ݷ��� �� �� ���� ȹ���ϰ� �������� ���ڿ� ��
        - ���� ���, �ٷ� ȹ��
    �÷��̾ �ִٸ� -> �ο�
        �ʱ� �ɷ�ġ + ���� �� ���ݷ� ���� ū ���
        -> ������, �ʱ� �ɷ�ġ�� ū ���
    �̱� ����� |(p1�ɷ�ġ + p1 �� ���ݷ�) - (p2�ɷ�ġ + p2 �� ���ݷ�)| ��ŭ ����Ʈ ȹ��
    �� ����� ������ �ִ� ���� ���ڿ� ����, ������ �̵�
        ���� ���̰ų� , ĭ�� �ٸ� ����� ������ ���������� 90�� ȸ�� �� �� ĭ�� ���� �� �̵�.
        ���������� �̵� �� �� ȹ�� �� ������ �� ���ڿ� ��
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
int dirs[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1} }; // �������
Player players[31];
vector<vector<vector<int>>> board(21, vector<vector<int>>(21, vector<int>()));

bool movePlayer(int pId) {
    auto& p = players[pId];

    int nr = p.row + dirs[p.dir][0];
    int nc = p.col + dirs[p.dir][1];

    if (nr > n || nc > n || nr < 1 || nc < 1) { // ���� ���
        p.dir = (p.dir + 2) % 4; // �ݴ� ��ȯ
        // �ٽ� 1ĭ �̵�
        nr = p.row + dirs[p.dir][0];
        nc = p.col + dirs[p.dir][1];
    }
    // �̵�
    p.row = nr;
    p.col = nc;
    return true;
}

void pickGun(int pId) {
    auto& curP = players[pId];
    if (!board[curP.row][curP.col].empty()) {
        //���� ������,
        int maxGun = 0;
        int maxId = 0;
        auto& guns = board[curP.row][curP.col];
        // �ִ� ���ݷ� �� ã��.
        for (int gId = 0; gId < guns.size(); gId++) {
            if (guns[gId] > maxGun) {
                maxGun = guns[gId];
                maxId = gId;
            }
        }

        // �÷��̾ ���� �ͺ��� ��� �� ����
        if (maxGun > curP.gun) {
            guns.erase(guns.begin() + maxId);
            if (curP.gun != 0) { // �÷��̾ ���� ������ �־�����
                guns.push_back(curP.gun); // ������ �ִ� ���� ����
            }
            curP.gun = maxGun; // �÷��̾� �� ������Ʈ
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
        // �ο�
        int win = -1;
        int lose = -1;

        auto& other = players[tId];

        int sum1 = curP.stat + curP.gun;
        int sum2 = other.stat + other.gun;

        if (sum1 > sum2) {
            //���� �̱�
            win = pId;
            lose = tId;
        }
        else if (sum1 == sum2) {
            if (curP.stat > other.stat) { // ���� �̱�
                win = pId;
                lose = tId;
            }
            else { // other�̱�
                win = tId;
                lose = pId;
            }
        }
        else {
            // other�̱�
            win = tId;
            lose = pId;
        }

        // ���� ������Ʈ
        score[win] += abs(sum1 - sum2);

        // �� ��� �ൿ ����
        auto& loser = players[lose];
        if (loser.gun > 0) { //���� ������ ������, ���� ����
            board[loser.row][loser.col].push_back(loser.gun);
            loser.gun = 0;
        }

        while (true) {
            int nr = loser.row + dirs[loser.dir][0];
            int nc = loser.col + dirs[loser.dir][1];

            if (nr > n || nc > n || nr < 1 || nc < 1) {
                loser.dir = (loser.dir + 1) % 4; // ������ 90�� ȸ��
                continue;
            }
            bool isExist = false;
            for (int i = 1; i <= m; i++) {
                if (players[i].row == nr && players[i].col == nc) {
                    isExist = true;
                    break;
                }
            }
            if (isExist) { // ������ ���� ����� �ִٸ�
                loser.dir = (loser.dir + 1) % 4; // ������ 90�� ȸ��
                continue;
            }

            // �����̰� break
            loser.row = nr;
            loser.col = nc;
            break;
        }
        pickGun(lose);

        // ���� �ൿ
        pickGun(win);
    }
    else {
        // �� ����
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
    // ���⿡ �ڵ带 �ۼ����ּ���.
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
