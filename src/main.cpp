#include <iostream>

using Node = int;

struct Edge {
    Node v, w;
    double cost;
};
constexpr double inf = 1e20;
constexpr int sz = 7;


struct Node {
    Key key;
    Node *left, *right;
    int h = 1;
    static int height(const Node* p) { return (p == nullptr) ? 0 : p->h; }
    void updateHeight() { h = std::max(height(left), height(right)) + 1; }

    int balanceFactor() const { return height(left) - height(right); }
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
        if (n == nullptr) { printf("not found?\n"); return; }
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
                remove(n->left, tmp->key);
            }
        }
        if (n == nullptr) return;
        balance(n);
    }
};


void prim(double mat[sz][sz], ssize_t len) {
    int* s = new int[len] {-1}, j;
    for (int i = 1; i < len; ++i) s[i] = 0;
    for (int k = 1; k < len; ++k) {
        double min = inf;
        for (int i = 0; i < len; ++i)
            if (s[i] != -1 && min > mat[i][s[i]]) {
                min = mat[i][s[i]];
                j = i;
            }
        std::cout << "edge: " << j << "-" << s[j]<< "\n";
        for (int i = 0; i < len; ++i)
            if (s[i] != -1 && mat[i][s[i]] > mat[i][j])
                s[i] = j;
        s[j] = -1;
    }
}


int main() {
    Edge edges[] = {
        {0, 1, 5},
        {0, 2, 8},
        {1, 2, 11},
        {1, 3, 10},
        {2, 3, 15},
        {2, 5, 9},
        {3, 4, 2},
        {3, 5, 7},
        {4, 5, 10},
        {4, 6, 4},
        {5, 6, 12},
    };
    double mat[sz][sz];
    for (int i = 0; i < sz; ++i)
        for (int j=0; j< sz;++j) mat[i][j] = inf;
    for (int i =0; i < sz;++i) mat[i][i] = 0;
    for (size_t i = 0; i < sizeof(edges)/sizeof(edges[0]); ++i){
        auto e = edges[i];
        mat[e.v][e.w] = e.cost;
        mat[e.w][e.v] = e.cost;
    }

    prim(mat, sz);
    
    return 0;
}
