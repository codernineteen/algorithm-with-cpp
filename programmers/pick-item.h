#pragma once

#include "types.h"

#define X first
#define Y second


void solve_pick_item() {
    using namespace std;

    int dx[4] = { 1,0,-1,0 };
    int dy[4] = { 0,1,0,-1 };

    int solution(vector<vector<int>> rectangle, int characterX, int characterY, int itemX, int itemY) {
        int answer = 0;
        //rect[0] : ÁÂÇÏ x
        //rect[1] : ÁÂÇÏ y
        vector<vector<int>> graph(104, vector<int>(104, 0));
        vector<vector<int>> vis(104, vector<int>(104, 0));
        // ±×·¡ÇÁ »ý¼º
        characterX *= 2;
        characterY *= 2;
        itemX *= 2;
        itemY *= 2;
        for (auto& rect : rectangle)
            for (int i = 0; i < 4; i++) rect[i] *= 2;

        for (auto& rect : rectangle) {
            for (int i = rect[1]; i <= rect[3]; i++) {
                if (graph[rect[0]][i] != 2) graph[rect[0]][i] = 1;
                if (graph[rect[2]][i] != 2) graph[rect[2]][i] = 1;
            }
            for (int j = rect[0]; j <= rect[2]; j++) {
                if (graph[j][rect[1]] != 2) graph[j][rect[1]] = 1;
                if (graph[j][rect[3]] != 2) graph[j][rect[3]] = 1;
            }

            for (int i = rect[0] + 1; i < rect[2]; i++) {
                for (int j = rect[1] + 1; j < rect[3]; j++) {
                    graph[i][j] = 2;
                }
            }
        }

        queue<pair<int, pair<int, int>>> nexts;
        nexts.push({ 0,{characterX, characterY} });

        while (!nexts.empty()) {
            auto data = nexts.front();
            nexts.pop();
            int cost = data.first;
            auto coord = data.second;
            int x = coord.X;
            int y = coord.Y;

            if (x == itemX && y == itemY) {
                answer = cost;
                break;
            }


            for (int dir = 0; dir < 4; dir++) {
                int nx = dx[dir] + x;
                int ny = dy[dir] + y;

                if (nx < 0 || nx >= 104 || ny < 0 || ny >= 104) continue;
                if (vis[nx][ny]) continue;
                if (graph[nx][ny] != 1) continue;

                vis[nx][ny] = 1;
                nexts.push({ cost + 1, {nx, ny} });
            }
        }


        return answer / 2;
    }
}