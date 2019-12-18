#pragma once
#include <initializer_list>
#include <iostream>
#include <fstream>

template<typename T>
constexpr size_t getSize(std::initializer_list<T> l) {
    size_t n = 0;
    auto it = l.begin();
    auto end = l.end();
    while (it++ != end) ++n;
    return n;
}

constexpr void getSize(std::initializer_list<
                           std::initializer_list<double>> list,
                       int& s1, int& s2) {
    constexpr int invalid = -1;
    s1 = 0;
    s2 = invalid;

    for (const auto& l: list) {
        int i = getSize(l);
        ++s1;
        if (s2 != invalid && i != s2) {
            //std::cerr << i << "!=" << s2 << "\n";
            throw std::logic_error("Fixed line size expected");
        }
        s2 = i;
    }
    //std::cerr << "size: " << s1 << ", " << s2 << "\n";
}
constexpr int MAX_SZ = 256;
inline size_t readSize(const char* name, int a = 1, int b = MAX_SZ) {
    int res;
    do {
        std::cout << name << ": ";
        std::cin >> res;
    } while (res < a || res > b);
    return res;
}


inline void assert(bool cond, const char* msg) {
    if (!cond) throw std::logic_error(msg);
}
template<typename T>
inline T read(const char* name) {
    T res;
    std::cout << name << ": ";
    std::cin >> res;
    return res;
}

template<typename It>
constexpr auto average(It begin, It end) {
    std::remove_const_t<std::remove_reference_t<decltype(*begin)>> sum {};
    size_t count = 0;
    while (begin != end) {
        sum += *begin;
        ++begin;
        ++count;
    }
    return sum /count;
}
size_t fileLineCount(std::ifstream& f) {
    size_t count = 0;
    while (!f.eof()) {
        if (auto c = f.get(); c == '\n') ++count;
    }
    return count;
}
