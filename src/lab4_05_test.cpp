#include "utils.h"
#include "lists.h"

#include <iostream>
#include <cstdlib>

bool isVowel(char c) {
    c = tolower(c);
    return c == 'a' || c== 'e' || c== 'i' || c=='o' || c=='u';
}

int main() {
    std::string str;
    std::cout << "str: ";
    std::getline(std::cin, str);

    char v;
    do {
        v = read<char>("vowel");
    } while (!isVowel(v));

    Stack<char> lower;
    Stack<char> upper;
    auto printStacks = [&] (const char* s) {
                           std::cout << s;
                           std::cout << "lower:" << lower << "\n";
                           std::cout << "upper:" << upper << "\n";
                       };

    printStacks("Before:\n");
    for (auto& c : str) {
        if (islower(c)) lower.push(c);
        else if (isupper(c)) upper.push(c);
    }

    printStacks("After Adding:\n");
    Stack<char>& stack = isupper(v)? upper: lower;
    while (!stack.empty()) {
        if (char c = stack.pop(); c == v) {
            break;
        }
    }
    printStacks("Result:\n");
    return 0;
}
