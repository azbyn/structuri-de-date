#pragma once
#include "utils.h"
#include "ptrRange.h"

#include <fstream>
#include <algorithm>

template<typename Key, typename T>
struct AVLTree {
    struct Node {
        Key key;
        //T val;
        Node* left;
        Node* right;
        int h = 1;

        Node(const Key& key,/* const T& val,*/ Node* left = nullptr, Node* right = nullptr)
            : key(key), /*val(val),*/ left(left), right(right) {}

        Node(const Node&) = delete;
        Node(Node&&) = delete;
        Node& operator=(const Node&) = delete;
        Node& operator=(Node&&) = delete;
        ~Node() {
            delete left;
            delete right;
        }

        static constexpr int height(const Node* p) {
            return (p == nullptr) ? 0 : p->h;
        }
        void updateHeight() {
            h = std::max(height(left), height(right)) + 1;
        }

        constexpr int balanceFactor() const {
            auto l = height(left);
            auto r = height(right);
            return l - r;
        }
        static void rightRotate(Node*& n) {
            Node* t = n->left;
            n->left = t->right;
            t->right = n;

            n->updateHeight();
            t->updateHeight();

            n = t;
        }

        static void leftRotate(Node*& n) {
            Node* t = n->right;
            n->right = t->left;
            t->left = n;
            t->updateHeight();
            n->updateHeight();
            n = t;
        }
        static void balance(Node*& node) {
            node->updateHeight();
            int factor = node->balanceFactor();
            if (factor > 1) {
                if (node->left->balanceFactor() < 0)
                    leftRotate(node->left);

                rightRotate(node);
            } else if (factor < -1) {
                if (node->right->balanceFactor() > 0)
                    rightRotate(node->right);

                leftRotate(node);
            }
        }
        static void add(Node*& node, const Key& key) {
            if (node == NULL) {
                node = new Node(key);
                return;
            }

            auto n = node;
            assert(key != n->key, "Value already in tree");
            if (key < node->key)
                add(node->left, key);
            else if (key > node->key)
                add(node->right, key);
            balance(node);
        }

        static void remove(Node*& n, const Key& key) {
            if (n == nullptr) {
                printf("not found?\n");
                return;
            }
            if (key < n->key) {
                remove(n->left, key);
            } else if (key > n->key) {
                remove(n->right, key);
            } else { // key == n->key
                if (n->left == nullptr) {
                    if (n->right == nullptr) {
                        n = nullptr;
                        return;
                    }
                    Node* tmp = n;
                    n = n->right;
                    tmp->right = nullptr;
                    delete tmp;
                } else if (n->right == nullptr) {
                    Node* tmp = n;
                    n = n->left;
                    tmp->left = nullptr;
                    delete tmp;
                } else {
                    constexpr auto findMax =
                        [](Node*& r, auto& findMax) -> Node*& {
                            if (r->right == nullptr) return r;
                            return findMax(r->right, findMax);
                        };
                    Node*& tmp = findMax(n->left, findMax);
                    n->key = tmp->key;
                    // std::swap(tmp->val, n->val);
                    remove(n->left, tmp->key);
                }
            }
            if (n == nullptr) return;
            balance(n);
        }
    };
    Node* root;

private:
    static void print(std::ostream& s, const Node* t, size_t indent = 0) {
        for (size_t i = 0; i < indent * 2; ++i) std::cout << ' ';
        if (t == nullptr) {
            s << "null\n";
            return;
        }
        s << "" << t->key << " - " <</* t->val*/ t->factor << "\n";
        if (t->left == nullptr && t->right == nullptr) return;
        print(s, t->left, indent + 1);
        print(s, t->right, indent + 1);
    }

public:
    AVLTree() : root(nullptr) {}
    AVLTree(std::initializer_list<Key> l) : root(nullptr) {
        for (auto& e : l) add(e);
    }

    ~AVLTree() {
        delete root;
    }
    struct NodeRef { Node*& val; };

    void add(const Key& key/*, const T& val*/) {
        Node::add(root, key);
    }


    friend std::ostream& operator<<(std::ostream& s, const AVLTree& t) {
        print(s, t.root);
        return s;
    }
    void printIndented() {
        printIndented(root);
    }
    static void printIndented(const Node* t, size_t indent = 0) {
        if (t == nullptr) return;
        printIndented(t->right, indent + 1);
        for (size_t i = 0; i < indent * 2; ++i) std::cout << ' ';
        if (t == nullptr) {
            std::cout << "null\n";
            return;
        }
        std::cout << "" << t->key << "; factor: " << t->balanceFactor() << "\n";
        //if (t->left == nullptr && t->right == nullptr) return;

        printIndented(t->left, indent + 1);
    }
    constexpr bool empty() const {
        return root == nullptr;
    }

    template <typename F>
    constexpr void visit(F f) const {
        auto impl = [&f](Node* t, auto& impl) -> void {
            if (t->left) impl(t->left, impl);
            f(t->val);
            if (t->right) impl(t->right, impl);
        };
        if (root == nullptr) return;
        impl(root, impl);
    }

    template <typename F>
    constexpr void visitWithKey(F f) const {
        auto impl = [&f](Node* t, auto& impl) -> void {
            if (t->left) impl(t->left, impl);
            f(t->key, t->val);
            if (t->right) impl(t->right, impl);
        };
        if (root == nullptr) return;
        impl(root, impl);
    }

    constexpr Node* find(const Key& key) {
        return findRef(key).val;
    }
    constexpr NodeRef findRefBalance(const Key& key) {
        auto impl = [&key](Node*& t, auto& impl) {
            if (t == nullptr || key == t->key)
                return NodeRef{ t };
            if (key < t->key) {
                return impl(t->left, impl);
            }
            return impl(t->right, impl);
        };
        return impl(root, impl);
    }
    constexpr NodeRef findRef(const Key& key) {
        auto impl = [&key](Node*& t, auto& impl) {
            if (t == nullptr || key == t->key)
                return NodeRef{ t };
            if (key < t->key) {
                return impl(t->left, impl);
            }
            return impl(t->right, impl);
        };
        return impl(root, impl);
    }
    void remove(const Key& key) {
        Node::remove(root, key);
    }

};
