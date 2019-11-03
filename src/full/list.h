#include "utils.h"

#include <type_traits>
#include <utility>


// We should never check for bot==nullptr,
//so we don't update it for a empty list
template<typename T>
struct List {
    struct Node {
        T val;
        Node* next;
    };
    Node* top;
    Node* bot;
    constexpr List(Node* top = nullptr, Node* bot = nullptr)
        : top(top), bot(bot) {}
    List(const List&) = delete;
    List& operator=(const List&) = delete;
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
        //if (top == nullptr) bot = nullptr;
        T res = n->val;
        delete n;
        return res;
    }

    void push_front(T val) {
        Node *n = new Node{ val, top };
        top = n;
        //  if (bot == nullptr)
    }
    void push_back(T val) {
        Node *n = new Node{ val, nullptr };
        if (top == nullptr) {
            top = n;
            //bot = n;
        }
        else {
            bot->next = n;

        }
        bot = n;
        //if (top == nullptr)
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
                //l->bot = nullptr;
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
