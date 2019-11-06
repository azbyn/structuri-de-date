#pragma once
#include "utils.h"

template<class T>
struct Queue {
    struct Node {
        T val;
        Node* next;
    };
    Node* top;
    Node* bot;
    Queue(Node* top = nullptr, Node* bot = nullptr) : top(top), bot(bot) {}
    /*
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;
    ~Queue() {
        while (top) {
            Node* n = top;
            top = top->next;
            delete n;
        }
        }*/
    void push(T val) {
        Node *n = new Node{ val, nullptr };
        if (bot == nullptr) {
            top = n;
        }
        else {
            bot->next = n;
        }
        bot = n;
    }

    T pop() {
        assert(top, "Empty queue");
        Node* old = top;
        T res = old->val;
        top = top->next;
        if (top == nullptr)
            bot = nullptr;
        delete old;
        return res;
    }
    bool empty() const { return top == nullptr; }
    static Queue read(const char* msg) {
        Queue q;
        std::cout << msg  << ":\n";
        int len1 = readSize("n");
        for (int i = 0; i < len1; ++i) {
            T s;
            std::cin >> s;
            q.push(s);
        }
        return std::move(q);
    }

    friend std::ostream& operator<<(std::ostream& s, const Queue& st) {
        if (st.empty()) return s << "{}";
        s << "{";
        const Node* it = st.top;
        while (it->next != nullptr) {
            s << it->val << ", ";
            it = it->next;
        }

        return s << it->val << "}";
    }
};
