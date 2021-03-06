#pragma once
#include "utils.h"
#include "ptrRange.h"

#include <type_traits>
#include <utility>


// We should never check for (bot == nullptr),
// so we don't update it when the list becomes empty.
template<typename T>
struct List {
    struct Node {
        T val;
        Node* next;
    };
    struct It {
        const Node* n;
        constexpr It(const Node* n) : n(n) {}
        constexpr It& operator++() {
            n = n->next;
            return *this;
        }
        constexpr auto& operator*() { return n->val; }
        constexpr bool operator ==(const It& rhs) { return n == rhs.n; }
        constexpr bool operator !=(const It& rhs) { return n != rhs.n; }
    };
    Node* top;
    Node* bot;
    constexpr List(Node* top = nullptr, Node* bot = nullptr)
        : top(top), bot(bot) {}
    List(const List&) = delete;
    List& operator=(const List&) = delete;

    constexpr It begin() const { return top; }
    constexpr It end() const { return nullptr; }

    constexpr const T& front() const { return top->val; }
    constexpr T& front() { return top->val; }
    constexpr size_t size() const {
        size_t sz = 0;
        Node* n = top;

        while (n) {
            ++sz;
            n = n->next;
        }
        return sz;
    }

    constexpr void copyTo(T* ptr) const {
        for (auto v : *this) {
            *(ptr++) = v;
        }
    }
    ManagedPtrRange<T> toManagedPtrRange() const {
        auto res = ManagedPtrRange<T>(size());
        copyToMemory(res.first);
        return res;
    }

    ~List() {
        while (top) {
            Node* n = top;
            top = top->next;
            delete n;
        }
    }
    List(List&& rhs) noexcept
        : top(std::exchange(rhs.top, nullptr)),
          bot(std::exchange(rhs.bot, nullptr)) {}
    List& operator=(List&& rhs) noexcept {
        this->~List();
        this->top = std::exchange(rhs.top, nullptr);
        this->bot = std::exchange(rhs.bot, nullptr);
    }

    T& first() { return top->val; }
    T& last() { return bot->val; }
    T pop_front() {
        assert(top, "Empty list");
        Node* n = top;
        top = top->next;
        T res = n->val;
        delete n;
        return res;
    }

    void push_front(T val) {
        Node *n = new Node{ val, top };
        top = n;
    }
    void push_back(T val) {
        Node *n = new Node{ val, nullptr };
        if (top == nullptr) {
            top = n;
        }
        else {
            bot->next = n;

        }
        bot = n;
    }

    template <typename... Args>
    void emplace_front(Args&&... args) {
        Node *n = new Node{ T(std::forward<Args>(args)...), top };
        top = n;
    }
    template <typename... Args>
    void emplace_back(Args&&... args) {
        Node *n = new Node{ T(std::forward<Args>(args)...), nullptr };
        if (top == nullptr) {
            top = n;
        }
        else {
            bot->next = n;

        }
        bot = n;
    }

    bool empty() const { return top == nullptr; }

    bool operator== (const List& rhs) {
        for (auto it = top, rit = rhs.top;
            it != nullptr;
            it = it->next, rit = rit->next) {
            if (it->val != rit->val) return false;
        }
        return true;
    }

    static List read(const char* msg) {
        List q;
        std::cout << msg << ":\n";
        int len1 = readSize("n");
        for (int i = 0; i < len1; ++i) {
            T s;
            std::cin >> s;
            q.push_front(s);
        }
        return std::move(q);
    }

    friend std::ostream& operator<<(std::ostream& s, const List& st) {
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
        remove_if([&] (T& t) { return t ==val; });
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

    template<typename P>
    const Node* find(P p) const {
        auto n = top;
        for (; n; n = n->next) {
            if (p(n->val))
                return n;
        }
        return n;
    }

    Node* findElem(T& e) {
        return find([&](T& other) { return other == e; });
    }
    const Node* findElem(T& e) const {
        return find([&](T& other) { return other == e; });
    }
};

// keeps in l all the elements that don't satisfy the predicate p
// and retuns pair of:
//  - a reference to the original list
//  - a list containing the elements that satisfy p
template<typename T, typename P>
constexpr auto partition_split(List<T>& l, P p) {
    struct res_t {
        List<T>& notSatisfying;
        List<T> satisfying;
    } res = { l, {} };
    List<T>& sat = res.satisfying;
    auto insertNode = [&sat](auto* n) {
        n->next = sat.top;
        sat.top = n;
        if (sat.bot == nullptr) {
            sat.bot = n;
        }
    };
    l.apply_on(p, insertNode);
    return res;
}
