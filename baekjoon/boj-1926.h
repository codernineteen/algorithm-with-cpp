#pragma once

#include "types.h"

#define X first
#define Y second

// 그림의 개수와 가장 큰 그림의 크기
// 그림 : 1로 연결된 것
// 그림의 넓이  : 그림에 포함된 1의 개수

int board[502][502] = { 0, };
bool vis[502][502] = { 0, };
int dx[4] = { 1,0,-1,0 };
int dy[4] = { 0,1,0,-1 };

void solve_1926() {
	int N, M;
	cin >> N >> M;

	queue<pair<int, int>> Q;

	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			int el;
			cin >> el;
			board[i][j] = el;
			if (el == 1) Q.push({ i,j });
		}
	}

	int maxArea = 0;
	int picCount = 0;

	while (!Q.empty()) {
		auto& now = Q.front();
		Q.pop();

		if (vis[now.X][now.Y]) continue;
		vis[now.X][now.Y] = 1;

		int area = 1;
		queue<pair<int, int>> subQ;
		subQ.push({ now.X, now.Y });

		while (!subQ.empty()) {
			auto& subNow = subQ.front();
			subQ.pop();

			for (int dir = 0; dir < 4; dir++) {
				int nx = subNow.X + dx[dir];
				int ny = subNow.Y + dy[dir];

				if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;
				if (vis[nx][ny] || board[nx][ny] == 0) continue;

				area++;
				vis[nx][ny] = 1;
				subQ.push({ nx, ny });
			}
		}

		picCount++;
		maxArea = max(area, maxArea);
	}

	cout << picCount << "\n";
	cout << maxArea;
}