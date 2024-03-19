#include "types.h"

void solve_1402() {
    list<char> li;
    char empty = ' ';
    string input;
    cin >> input;
    for (auto c : input) {
        li.push_front(empty);
        li.push_front(c);
    }
    li.push_front(empty);

    int times;
    cin >> times;

    auto it = li.begin();
    while (times-- > 0) {

        char cmd;
        cin >> cmd;

        if (cmd == 'P') {
            char newChar;
            cin >> newChar;
            li.insert(it, empty);
            li.insert(it, newChar);
            advance(it, -2);
        }
        else if (cmd == 'L') {
            //cout << "yes";
            auto last = li.end();
            last--;
            if (it != last) advance(it, 2);
        }
        else if (cmd == 'D') {
            if (it != li.begin()) advance(it, -2);
        }
        else { //»èÁ¦
            auto last = li.end();
            last--;
            if (it != last) {
                it = li.erase(it);
                it = li.erase(it);
            }
        }

    }

    string res = "";
    for (auto it = li.rbegin(); it != li.rend(); it++) {
        if (*it != empty) res += *it;
    }

    cout << res << "\n";
}