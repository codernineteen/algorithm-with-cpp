#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

int bin_search(int s, int e, int t, const vector<pair<int,int>>& arr) {
	int mid = (s + e) / 2;

	while (s <= e) {
		mid = (s + e) / 2;

		if (arr[mid].first == t) return arr[mid].second;
		else if (arr[mid].first > t) {
			e = mid - 1;
		}
		else {
			s = mid + 1;
		}
	}

	return 0;
}

int solve()
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N, M;
	cin >> N;
	
	vector<int> arr;
	arr.resize(N);

	for (int i = 0; i < N; i++)
		cin >> arr[i];

	sort(arr.begin(), arr.end());

	vector<pair<int,int>> compress;

	int prev = INT_MIN;
	for (int i = 0; i < N; i++) {
		if (arr[i] == prev) compress[compress.size()-1].second++;
		else compress.push_back({ arr[i], 1 });

		prev = arr[i];
	}

	cin >> M;
	while (M--) {
		int t;
		cin >> t;
		cout << bin_search(0, compress.size() - 1, t, compress) << " ";
	}


	return 0;
}
