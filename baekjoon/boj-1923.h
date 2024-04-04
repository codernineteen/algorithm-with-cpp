#include "types.h"

using namespace std;

// 명제 : 현재 시간이 t라고 할 때 시작 시간이 t 이상인 모든 회의 중에서 끝나는 시간이 가장 빠른 회의를 선택하면 최적의 해를 구할 수 있다.
// 증명 : 회의 1 [a,b] / 회의 2 [c,d] , a <= b <= c <= d 라고 할 때 회의 1을 선택하지 않고 회의 2를 선택했을 때 최적의 해라고 가정하자.
// 하지만, 너무 자명하게도 회의 2를 선택하더라도 회의 1을 동시에 선택할 수 있으므로 1을 먼저 선택하는게 최적해이다. 이는 가정의 모순이므로 증명 완료
 
int solve() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	int N;
	cin >> N;

	vector<pair<int, int>> meets(N);
	for (int i = 0; i < N; i++) {
		cin >> meets[i].first >> meets[i].second;
	}

	sort(meets.begin(), meets.end(), [=](const pair<int, int>& a, const pair<int, int>& b) {
		return a.second == b.second ? a.first < b.first : a.second < b.second;
		}); // 끝나는 시간이 빠른 순으로 정렬, 같으면 시작하는 시간이 빠른 순으로 정렬 (시작 시간과 끝나는 시간이 같은 공짜 타임들을 추가해주기 위함)

	int last = 0;
	int cnt = 0;
	for (auto& meet : meets) {
		if (meet.first >= last) {
			cnt++;
			last = meet.second;
		}
	}

	cout << cnt << "\n";
}
