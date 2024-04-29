#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <climits>
#include <queue>
#include <algorithm>
#include <functional>
#include <tuple>
#include <array>


using namespace std;

struct Cell {
	int h;
	int rem;
	bool wait;
	bool rep;
};

int dirs[4][2] = { {-1,0}, {1,0}, {0,-1}, {0,1} };
int N, M, K;
Cell board[351][351] = { {Cell{0,0,0,0}} };

void printBoard() {
	for (int i = 130; i <= 160; i++) {
		for (int j = 130; j <= 160 + N; j++) {
			cout << board[i][j].h << " ";
		}
		cout << "\n";
	}
}

int solve() {
	cin >> N >> M >> K;

	for (int i = 150; i < 150 + N; i++) {
		for (int j = 150; j < 150 + M; j++) {
			int num;
			cin >> num;

			if (num > 0) {
				board[i][j] = Cell{ num, num, true, false };
			}
		}


	}


	int t = 0;
	while (t <= K) {
		for (int i = 0; i <= 350; i++) {
			for (int j = 0; j <= 350; j++) {
				if (board[i][j].h && board[i][j].rem && !board[i][j].wait) {
					if (!board[i][j].rep) {
						for (auto& dir : dirs) {
							int nr = i + dir[0];
							int nc = j + dir[1];

							if (board[nr][nc].h < 0) continue;
							if (board[nr][nc].h > 0 && !board[i][j].wait) continue;

							//// 번식 가능
							if (board[nr][nc].h == 0) {
								board[nr][nc] = Cell{ board[i][j].h, board[i][j].h, true, false };
							}
							else if (board[nr][nc].h > 0 && board[i][j].wait) {
								if (board[i][j].h > board[nr][nc].h) {
									board[nr][nc] = Cell{ board[i][j].h, board[i][j].h, true, false };
								}
							}
						}
					}
				}
			}
		}
		printBoard();
		t++;

		for (int i = 0; i <= 350; i++) {
			for (int j = 0; j <= 350; j++) {
				if (board[i][j].h > 0 && board[i][j].rem > 0 && board[i][j].wait) {
					board[i][j].rem--;

					if (board[i][j].rem == 0) {
						board[i][j].rem = board[i][j].h;
						board[i][j].wait = false;
					}
				}
				else if (board[i][j].h > 0 && board[i][j].rem > 0 && !board[i][j].wait) {
					board[i][j].rem--;

					if (board[i][j].rem == 0) {
						board[i][j].h = -1;
					}
				}

			}
		}

		printBoard();
	}


	int res = 0;
	for (int i = 0; i <= 350; i++) {
		for (int j = 0; j <= 350 + N; j++) {
			if (board[i][j].h > 0) res++;
		}
	}
	
	cout << res << endl;

	return 0;//정상종료시 반드시 0을 리턴해야합니다.
}
