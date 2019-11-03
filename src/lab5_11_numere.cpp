#include "utils.h"
#include "list.h"

#include <iostream>

int main() {
    auto nums = List<int>::read("numbers");

    auto [evens, odds] = partition_split(nums, [](int n) {return n % 2;});
    std::cout << "odd:  " << odds << "\n";
    std::cout << "even: " << evens << "\n";

    int x;
    do {
        std::cout << "x (must be even): ";
        std::cin >> x;
    } while (x % 2);

    std::cout << "removing...\n";
    evens.remove(x);

    std::cout << "even: " << evens << "\n";
    return 0;
}
