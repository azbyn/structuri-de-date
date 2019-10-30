#include "utils.h"
#include "lists.h"

#include <array>
#include <chrono>

constexpr char toDigit(int i) {
    return (i < 10 ? '0' : ('A' - 10)) + i;
};

void betterToBase(int n, int b, char* res) {
    int digits = 0;
    int nSaved = n;
    while (n) {
        ++digits;
        n /= b;
    }
    char* p = res+digits;
    *p = '\0';
    if (b > 10) {
        while (nSaved) {
            *(--p) = toDigit(nSaved % b);
            nSaved /= b;
        }
    } else {
        while (nSaved) {
            *(--p) = '0' + (nSaved % b);
            nSaved /= b;
        }
    }
}

void to_base(int n, int b, char* res) {
    Stack<int> s;
    do {
        s.push(n % b);
        n /= b;
    } while(n);

    char* p = res;
    while (!s.empty()) {
        *(p++) = toDigit(s.pop());
    }
    *p = 0;
}

int main() {
    int base = readSize("base", 2, 16);
    int n = read<int>("n");
    char buf[65];

    to_base(n, base, buf);
    std::cout << "in base " << base << ": " << buf << "\n";
    betterToBase(n, base, buf);
    return 0;
}
