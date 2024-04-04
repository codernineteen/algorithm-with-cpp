#include "types.h"

using namespace std;

// ���� : ���� �ð��� t��� �� �� ���� �ð��� t �̻��� ��� ȸ�� �߿��� ������ �ð��� ���� ���� ȸ�Ǹ� �����ϸ� ������ �ظ� ���� �� �ִ�.
// ���� : ȸ�� 1 [a,b] / ȸ�� 2 [c,d] , a <= b <= c <= d ��� �� �� ȸ�� 1�� �������� �ʰ� ȸ�� 2�� �������� �� ������ �ض�� ��������.
// ������, �ʹ� �ڸ��ϰԵ� ȸ�� 2�� �����ϴ��� ȸ�� 1�� ���ÿ� ������ �� �����Ƿ� 1�� ���� �����ϴ°� �������̴�. �̴� ������ ����̹Ƿ� ���� �Ϸ�
 
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
		}); // ������ �ð��� ���� ������ ����, ������ �����ϴ� �ð��� ���� ������ ���� (���� �ð��� ������ �ð��� ���� ��¥ Ÿ�ӵ��� �߰����ֱ� ����)

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
