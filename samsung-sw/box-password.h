#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
#include <set>
#include <unordered_map>

using namespace std;

// N : 4의 배수, 8이상 28이하
// N개의 숫자는 각각 0이상 F이하 중하나.

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

		int quo = N / 4; // 한 변에 포함된 숫자 개수

		for (int i = 1; i <= N; i++) {
			char numChar;
			cin >> numChar;
			deq.push_back(um[numChar]);
		}

		int rotateCnt = 0;
		while (rotateCnt < N) { // 각 숫자가 회전 후 원래 자리로 돌아오기 전까지
			// 변 길이에 따라 슬라이싱
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

			// 회전
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
