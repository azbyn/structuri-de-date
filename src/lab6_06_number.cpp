#include "doubleList.h"
#include "utils.h"

#include <iostream>

int main() {
    DoubleList<char> list;

    std::cout << "n: ";
    char c;
    do {
        c= std::cin.get();
    } while (c < '1' || c > '9');
    for (;;) {
        c = std::cin.get();
        if (c >= '0' && c <= '9') {
            list.push_back(c);
        }
        else break;
    }

    std::cout << "n: '" << list << "'\n";

    //palindrome
    auto isPalindrome = [&] {
        auto it = list.top;
        auto rit = list.bot;

        for (;;) {
            if (it == nullptr) break;
            else if (it == rit) break;
            else if (rit->next == it) break;
            if (it->val != rit->val) return false;
            it = it->next;
            rit = rit->prev;
        }
        return true;
    };
    std::cout << "isPalindrome: " << (isPalindrome() ? "yes" : "no") << "\n";

    //c
    for (auto it = list.top; it; it = it->next->next) {
        if (it->next == nullptr) break;
        list.insert(it, std::max(it->val, it->next->val));

    }
    std::cout << "after c: '" << list << "'\n";

    //d(elete)

    bool found[10] = {};
    for (auto it = list.top; it; ) {

        int i = it->val - '0';
        if (found[i]) {
            if (it == list.bot) { list.pop_back(); break; }

            it->prev->next = it->next;
            it->next->prev = it->prev;
            auto newit = it->next;
            delete it;
            it = newit;
        }
        else {
            it = it->next;
        }
        found[i] = true;

    }
    std::cout << "after d: '" << list << "'\n";

    std::cout << "size: " << list.size() << "\n";

    return 0;
}
