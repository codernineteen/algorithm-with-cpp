#include <vector>
#include <unordered_set>
#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

int K, V, E;
vector<vector<int>> graph(20001, vector<int>());

bool check_adj(unordered_set<int> s) {
    for (auto& n : s) {
        for (auto& v : graph[n]) {
            if (s.find(v) != s.end()) return true;
        }
    }

    return false;
}

int solve()
{
    ios::sync_with_stdio(0);
    cin.tie(0);

    cin >> K;

    while (K--) {
        fill(graph.begin(), graph.end(), vector<int>());

        cin >> V >> E;
        while (E--) {
            int u, v;
            cin >> u >> v;
            graph[u].push_back(v);
            graph[v].push_back(u);
        }

        unordered_set<int> s1;
        unordered_set<int> s2;

        vector<bool> vis(V + 1, 0);
        for (int i = 1; i <= V; i++) {
            queue<int> q;
            vis[i] = 1;
            q.push(i);

            while (!q.empty()) {
                int cur = q.front();
                q.pop();

                for (auto& n : graph[cur]) {
                    if (vis[n]) continue;

                    if (s1.find(cur) != s1.end())
                        s2.insert(n);
                    else
                        s1.insert(n);

                    vis[n] = 1;
                    q.push(n);
                }
            }
        }

        if (!check_adj(s1) && !check_adj(s2)) cout << "YES" << "\n";
        else cout << "NO" << "\n";
    }

   
    return 0;
}

//2
//3 2
//1 3
//2 3
//2 1
//1 2
