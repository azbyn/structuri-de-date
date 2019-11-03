#include "utils.h"

template<typename T>
struct Stack {
    struct Node {
        T val;
        Node* next;
    };

    Stack(const Stack&) = delete;
    Stack& operator=(const Stack&) = delete;
    Node* top;
    Stack(Node* top = nullptr) : top(top) {}
    ~Stack() {
        while (top) {
            Node* n = top;
            top = top->next;
            delete n;
        }
    }
    void push(T val) {
        Node *n = new Node{ val, top };
        top = n;
    }
    void quickPop() {
        Node* n = top;
        top = top->next;
        delete n;
    }
    T pop() {
        assert(top, "Empty stack");
        Node* n = top;
        top = top->next;
        T res = n->val;
        delete n;
        return res;
    }
    bool empty() const { return top == nullptr; }
    friend std::ostream& operator<<(std::ostream& s, const Stack& st) {
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
