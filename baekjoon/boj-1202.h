#pragma once

#include "types.h"

#define MAX 300001
#define u64 uint64_t

using namespace std;

inline bool pair_comp(const pair<int, int>& a, const pair<int, int>& b) 
{
	return a.first < b.first;
}

void solve() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N, K;
	cin >> N >> K;

	
	pair<int, int> jewels[MAX];
	int bags[MAX];
	priority_queue<int> pq;

	for (int i = 0; i < N; i++)
	{
		int M, V;
		cin >> M >> V;
		jewels[i] = { M, V };
	}

	for (int i = 0; i < K; i++)
	{
		int C;
		cin >> C;
		bags[i] = C;
	}

	sort(jewels, jewels+N, pair_comp);
	sort(bags, bags+K);

	u64 res = 0;
	int it = 0;
	for (int i = 0;  i < K; i++)
	{
		while (it < N && jewels[it].first <= bags[i])
		{
			pq.push(jewels[it].second);
			it++;
		}

		if (!pq.empty())
		{
			res += pq.top();
			pq.pop();
		}
	}

	cout << res << "\n";
}
