#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <unordered_map>

using namespace std;

// N : 4�� ���, 8�̻� 28����
// N���� ���ڴ� ���� 0�̻� F���� ���ϳ�.

int N, K;
set<int, greater<int>> nums;
deque<int> deq;
unordered_map<char, int> um = {
	{'0', 0},
	{'1', 1},
	{'2', 2},
	{'3', 3},
	{'4', 4},
	{'5', 5},
	{'6', 6},
	{'7', 7},
	{'8', 8},
	{'9', 9},
	{'A', 0xA},
	{'B', 0xB},
	{'C', 0xC},
	{'D', 0xD},
	{'E', 0xE},
	{'F', 0xF},
};
vector<int> ans;

int solve() {
	int test_case;
	int T;

	cin >> T;
	for (test_case = 1; test_case <= T; ++test_case)
	{
		cin >> N >> K;

		int quo = N / 4; // �� ���� ���Ե� ���� ����

		for (int i = 1; i <= N; i++) {
			char numChar;
			cin >> numChar;
			deq.push_back(um[numChar]);
		}

		int rotateCnt = 0;
		while (rotateCnt < N) { // �� ���ڰ� ȸ�� �� ���� �ڸ��� ���ƿ��� ������
			// �� ���̿� ���� �����̽�
			for (int stride = 0; stride < 4; stride++) {
				int offset = stride * quo;

				int curNum = 0;
				int digit = 1;

				for (int i = quo-1; i >= 0; i--) {
					curNum += deq[offset+i] * digit;
					digit *= 16;
				}
				nums.insert(curNum);
			}

			// ȸ��
			int back = deq.back();
			deq.pop_back();
			deq.push_front(back);
			rotateCnt++;
		}



		auto it = nums.begin();
		advance(it, K-1);
		ans.push_back(*it);

		nums.clear();
		deq.clear();
	}
	
	for (int i = 0; i < ans.size(); i++) {
		cout << "#" << i + 1 << " " << ans[i] << "\n";
	}
	return 0;
}
