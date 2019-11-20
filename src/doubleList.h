#pragma once
#include "utils.h"

#include <utility>

template<typename T>
struct DoubleList {
    struct Node {
        T val;
        Node* next;
        Node* prev;
    };
    struct It {
        const Node* n;
        constexpr It(const Node* n) : n(n) {}
        constexpr It& operator++() { n = n->next; return *this; }
        constexpr It& operator--() { n = n->prev; return *this; }

        constexpr It operator++(int) { auto r = n; ++(*this); return r; }
        constexpr It operator--(int) { auto r = n; --(*this); return r; }

        constexpr auto& operator*() { return n->val; }
        constexpr auto& operator->() { return n->val; }
        constexpr bool operator ==(const It& rhs) { return n == rhs.n; }
        constexpr bool operator !=(const It& rhs) { return n != rhs.n; }
        constexpr bool hasNext() const { return n->next; }
        constexpr bool hasPrev() const { return n->prev; }
    };
    Node* top;
    Node* bot;
    constexpr DoubleList(Node* top = nullptr, Node* bot = nullptr)
        : top(top), bot(bot) {}
    DoubleList(std::initializer_list<T> l) : DoubleList() {
        for (auto& val : l) push_back(val);
    }
    DoubleList(const DoubleList&) = delete;
    DoubleList& operator=(const DoubleList&) = delete;

    constexpr It begin() const { return top; }
    constexpr It end() const { return nullptr; }

    ~DoubleList() {
        while (top) {
            Node* n = top;
            top = top->next;
            delete n;
        }
    }
    DoubleList(DoubleList&& rhs) noexcept
        : top(std::exchange(rhs.top, nullptr)),
          bot(std::exchange(rhs.bot, nullptr)) {}
    DoubleList& operator=(DoubleList&& rhs) noexcept {
        this->~DoubleList();
        this->top = std::exchange(rhs.top, nullptr);
        this->bot = std::exchange(rhs.bot, nullptr);
    }

    T& first() { return top->val; }
    T& last() { return bot->val; }
    T pop_front() {
        assert(top, "Empty list");
        Node* n = top;
        top = top->next;
        if (top == nullptr) bot = nullptr;
        top->prev = nullptr;
        T res = n->val;
        delete n;
        return res;
    }

    T pop_back() {
        assert(bot, "Empty list");
        Node* n = bot;
        bot = bot->prev;
        if (bot == nullptr) bot = nullptr;
        bot->next = nullptr;
        T res = n->val;
        delete n;
        return res;
    }

    void push_front(T val) {
        Node *n = new Node{ val, top, nullptr };
        if (top == nullptr) bot = n;
        else top->prev = n;
        top = n;
    }
    void push_back(T val) {
        Node *n = new Node{ val, nullptr, bot };
        if (top == nullptr) top = n;
        else bot->next = n;
        bot = n;
    }
    bool empty() const { return top == nullptr; }

    bool operator==(const DoubleList& rhs) {
        for (auto it = top, rit = rhs.top;
            it != nullptr;
            it = it->next, rit = rit->next) {
            if (it->val != rit->val) return false;
        }
        return true;
    }

    static DoubleList read(const char* msg) {
        DoubleList q;
        std::cout << msg << ":\n";
        int len1 = readSize("n");
        for (int i = 0; i < len1; ++i) {
            T s;
            std::cin >> s;
            q.push_back(s);
        }
        return std::move(q);
    }

    friend std::ostream& operator<<(std::ostream& s,
                                    const DoubleList& st) {
        if constexpr (std::is_same_v<T, char>) {
            const Node* it = st.top;
            while (it != nullptr) {
                s << it->val;
                it = it->next;
            }
            return s;
        }
        else {
            if (st.empty()) return s << "{}";
            s << "{";
            const Node* it = st.top;
            while (it->next != nullptr) {
                s << it->val << ", ";
                it = it->next;
            }
            return s << it->val << "}";
        }
    }

    void remove(T& val) {
        remove_if([&] (T& t) { return t == val; });
    }
    void remove(Node* n) {
        if (!n->prev) { std::cout << "FRONT\n"; pop_front(); return; }
        if (!n->next) { std::cout << "BACK\n"; pop_back(); return; }
        n->prev->next = n->next;
        n->next->prev = n->prev;
    }
    // removes all elements that satisfy p
    template<class P>
    void remove_if(P p) {
        apply_on(p, [] (Node* n) { delete n; } );
    }
    //applies f() on all nodes that satisfy the predicate p()
    template<class P, class F>
    void apply_on(P p, F f) {
        for (;;) {
            auto* n = top;
            if (!n) {
                return;
            }
            if (!p(n->val)) break;
            top = n->next;
            f(n);
        }
        auto* prev = top;
        auto* it = prev->next;

        while (it) {
            if (p(it->val)) {
                prev->next = it->next;
                if (prev->next == nullptr) {
                    bot = prev;
                }
                f(it);
                it = prev;
            } else {
                prev = it;
                it = it->next;
            }
        }
    }
    // P is a predicate on T
    template<typename P>
    Node* find(P p) {
        auto n = top;
        for (; n; n = n->next) {
            if (p(n->val))
                return n;
        }
        return n;
    }
};

