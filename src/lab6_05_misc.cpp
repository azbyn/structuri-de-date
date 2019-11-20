#include "doubleList.h"
#include "utils.h"

#include <iostream>

template<typename T>
void printBothWays(typename DoubleList<T>::Node *n) {
    if (n == nullptr) return;
    typename DoubleList<T>::It it(n);
    std::cout << "Forward:\n";

    while (it.hasNext()) {
        std::cout << *it++ << ", ";
    }
    std::cout << *it << "\n";
    std::cout << "Reverse:\n";
    while (it.hasPrev()) {
        std::cout << *it << ", ";
        --it;
    }
    std::cout << *it << "\n";
}

template<typename T>
void insert(typename DoubleList<T>::Node *n, T val) {
    using Node = typename DoubleList<T>::Node;
    Node* newN = new Node{ val, n->next, n};
    n->next->prev = newN;
    n->next = newN;
}

template<typename T>
void remove(typename DoubleList<T>::Node *n) {
    assert(n->prev && n->next, "Please call list.remove(n).");
    n->prev->next = n->next;
    n->next->prev = n->prev;
}

int main() {
    DoubleList<int> list = {1, 2, 5, 4 };
    insert(list.top->next, 3);
    std::cout << list << "\n";
    remove<int>(list.bot->prev);

    printBothWays<int>(list.top);

    return 0;
}
