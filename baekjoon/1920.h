#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int solve()
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N, M;
	cin >> N;
	vector<int> arr;
	arr.reserve(N);

	for (int i = 0; i < N; i++) {
		int t; 
		cin >> t;
		arr.push_back(t);
	}

	sort(arr.begin(), arr.end());

	cin >> M;
	for (int i = 0; i < M; i++) {
		int t; 
		cin >> t;
		bool res = binary_search(arr.begin(), arr.end(), t);
		if (res) cout << "1\n";
		else cout << "0\n";
	}


	return 0;
}
