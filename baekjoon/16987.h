#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

int N;
vector<int> rem;
vector<int> w;
int ans = 0;

void fn(int l, int cnt) {
	if (l == N) {
		ans = max(ans, cnt);
		return;
	}

	for (int i = 0; i < N; i++) {	
		if (l == i) continue;
		if (rem[l] <= 0 || rem[i] <= 0) {
			fn(l + 1, cnt);
		}
		else {
			rem[l] -= w[i];
			rem[i] -= w[l];

			int delta = 0;
			if (rem[l] <= 0) delta++;
			if (rem[i] <= 0) delta++;
			fn(l + 1, cnt + delta);

			rem[l] += w[i];
			rem[i] += w[l];
		}
	}
}

int solve()
{
	ios::sync_with_stdio(0);
	cin.tie(0);

	cin >> N;
	rem.resize(N); w.resize(N);
	for (int i = 0; i < N; i++) {
		cin >> rem[i] >> w[i];
	}

	fn(0, 0);

	cout << ans << "\n";

	return 0;
}
