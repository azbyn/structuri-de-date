#pragma once

#include "utils.h"
#include "ptrRange.h"
#include <fstream>

template<typename Key, typename T>
struct BinarySearchTree {

    struct Node {
        Key key;
        T val;
        Node* left;
        Node* right;

        Node(const Key& key, const T& val, Node* left = nullptr, Node* right = nullptr)
            : key(key), val(val), left(left), right(right) {}

        Node(const Node&) = delete;
        Node(Node&&) = delete;
        Node& operator=(const Node&) = delete;
        Node& operator=(Node&&) = delete;
        ~Node() {
            //might be a problem
            delete left;
            delete right;
        }
    };
    Node* root;

private:
    static void print(std::ostream& s, const Node* t, size_t indent = 0) {
        for (size_t i = 0; i < indent * 2; ++i) std::cout << ' ';
        if (t == nullptr) {
            std::cout << "null\n";
            return;
        }
        std::cout << "" << t->key << " - " << t->val << "\n";
        if (t->left == nullptr && t->right == nullptr) return;
        print(s, t->left, indent + 1);
        print(s, t->right, indent + 1);
    }
public:
    BinarySearchTree(): root(nullptr) {}

    ~BinarySearchTree() {
        delete root;
    }

    friend std::ostream& operator<<(std::ostream& s, const BinarySearchTree& t) {
        print(s, t.root);
        return s;
    }
    constexpr bool empty() const {
        return root == nullptr;
    }

    template <typename F>
    constexpr void visit(F f) const {
        auto impl = [&f] (Node* t, auto& impl) -> void {
                        if (t->left) impl(t->left, impl);
                        f(t->val);
                        if (t->right) impl(t->right, impl);
                    };
        if (root == nullptr) return;
        impl(root, impl);
    }

    template <typename F>
    constexpr void visitWithKey(F f) const {
        auto impl = [&f] (Node* t, auto& impl) -> void {
                        if (t->left) impl(t->left, impl);
                        f(t->key, t->val);
                        if (t->right) impl(t->right, impl);
                    };
        if (root == nullptr) return;
        impl(root, impl);
    }

    void add(const Key& key, const T& val) {
        auto res = findRef(key);
        assert(res.val == nullptr, "Value already in tree");
        add(res, key, val);
    }
    struct NodeRef { Node*& val; };
    void add(NodeRef n, const Key& key, const T& val) {
        n.val = new Node(key, val);
    }
    constexpr Node* find(const Key& key) {
        return findRef(key).val;
    }

    constexpr NodeRef findRef(const Key& key) {
        auto impl = [&key] (Node*& t, auto& impl) {
                                  if (t == nullptr || key == t->key)
                                      return NodeRef { t};
                                  if (key < t->key) {
                                      return impl(t->left, impl);
                                  }
                                  return impl(t->right, impl);
                              };
        return impl(root, impl);
    }
    constexpr void remove(NodeRef r) {
        assert(r.val != nullptr, "Node not found");
        Node*& n = r.val;
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
            constexpr auto findMax = [] (Node*& r, auto& findMax) -> Node*& {
                               if (r->right == nullptr) return r;
                               return findMax(r->right, findMax);
                           };
            Node*& replacement = findMax(n->left, findMax);
            Node* tmp = replacement;
            std::swap(replacement->key, n->key);
            std::swap(replacement->val, n->val);
            replacement = replacement->left;
            tmp->right = tmp->left = nullptr;
            delete tmp;
        }
    }
    constexpr void remove(const Key& key) { remove(findRef(key)); }
};
