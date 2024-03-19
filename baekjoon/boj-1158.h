#pragma once

#include "types.h"

// 요세푸스 문제

void solve_1158() {
	int N, K;
	cin >> N >> K;
	
	deque<int> deq;
	for (int i = 1; i <= N; i++) deq.push_back(i);

	cout << "<";
	while (true) {
		if (deq.size() == 1) break;

		int cnt = K - 1;
		while (cnt-- > 0) {
			int temp = deq.front();
			deq.pop_front();
			deq.push_back(temp);
		}

		cout << deq.front() << ", ";
		deq.pop_front();
	}
	cout << deq.back() << ">";
}


// 1 2 3 4 5 6 7