#pragma once
#include "utils.h"
#include "ptrRange.h"

#include <iostream>
#include <utility>

template<typename Val>
struct MaxHeap {
    Vector<Val> data;

    MaxHeap() : data() {}
    MaxHeap(const MaxHeap& rhs) : data(rhs.data) {}

    MaxHeap clone() const { return MaxHeap(*this); }

    constexpr static int parent(int i) { return (i - 1) / 2; }
    constexpr static int left(int i) { return (2 * i + 1); }
    constexpr static int right(int i) { return (2 * i + 2); }
    constexpr bool empty() const { return data.empty(); }
    void push(const Val& v) {
        data.push_back(v);
        for (size_t i = data.size()-1; ;) {
            if (i == 0) break;
            size_t j = parent(i);
            if (data[i] > data[j]) {
                std::swap(data[i], data[j]);
                i = j;
            }
        }
    }
    Val pop() {
        Val res = std::move(data[0]);
        data[0] = data.back();
        data.pop_back();
        size_t i = 0;
        size_t sz = data.size();
        while (i < sz) {
            size_t j = left(i);
            if (j < sz) {
                if (j+1 < sz && data[j+1] > data[j]) ++j;
                if (data[i] < data[j]) {
                    std::swap(data[i], data[j]);
                    i = j;
                }
                else break;
            } else break;
        }
        return res;

    }
    void printSorted() const {
        MaxHeap h = this->clone();
        while (!h.empty()) {
            auto val = h.pop();
            std::cout << val << "\n";
        }
    }
};
