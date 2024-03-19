#include "types.h"

void solve_5397() {
	int M;
	cin >> M;


	while (M-- > 0) {
		string str;
		cin >> str;

		list<char> li;
		auto it = li.begin();
		li.insert(it, ' ');
		it--;

		for (auto c : str) {
			if (c == '<') {
				if (it != li.begin()) advance(it, -2);
			}
			else if (c == '>') {
				auto end = li.end();
				end--;
				if (it != end) advance(it, 2);
			}
			else if (c == '-') {
				if (it != li.begin()) {
					advance(it, -2);
					it = li.erase(it);
					it = li.erase(it);
				}
			}
			else {
				li.insert(it, ' ');
				li.insert(it, c);
			}
		}

		string res = "";
		for (auto iter = li.begin(); iter != li.end(); iter++)
			if (*iter != ' ') res += *iter;

		cout << res << "\n";
	}
}
