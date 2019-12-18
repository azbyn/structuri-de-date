#include "utils.h"
#include "ptrRange.h"
#include <stdio.h>


struct Tree {
    using Node = unsigned;
    Node n;
    ManagedPtrRange<Tree*> children;

    Tree(Node n) : n(n), children(nullptr) {}

    Tree(const Tree&) = delete;
    Tree(Tree&&) = delete;
    Tree& operator=(const Tree&) = delete;
    Tree& operator=(Tree&&) = delete;
    ~Tree() {
        for (auto c : children)
            delete c;
    }

    struct Parser {
        std::ifstream f;
        Tree* res;
        size_t numNodes;
        size_t line;
        Parser(const char* path) : f(path, std::ios::in | std::ios::binary) {
            body(readNumNodes());
        }
        size_t readNumNodes() {
            enforceKeyword("nodes");
            enforceKeyword(":");
            size_t res = enforceVal<size_t>("Integer expected");
            enforceKeyword(";");
            return res;
        }
        void body(size_t numNodes) {
            std::cout << "nn: " << numNodes << "\n";

            Node *currChildrenBuffer = (Node*) alloca(sizeof(Node)*numNodes);
            Tree **trees = (Tree**)alloca(sizeof(Tree*)*numNodes);
            for (Node i = 0; i < numNodes; ++i)
                trees[i] = new Tree(i);
            res = trees[0];
            for (;;) {
                if (eatSpace()) return;
                readTreeChildren(currChildrenBuffer, trees);
            }
        }
        void readTreeChildren(Node* childrenBuffer, Tree** trees) {
            Node n = enforceVal<Node>("Expected node (int)");
            enforceKeyword(":");
            size_t numNodes = 0;
            if (checkChar(';')) return;
            for (;;) {
                if (eatSpace()) syntaxError("Expected ;");
                childrenBuffer[numNodes++] = enforceVal<Node>("Expected child");
                if (checkChar(';')) break;
                enforceKeyword(",");
            }
            trees[n]->children = ManagedPtrRange<Tree*>(numNodes);
            for (size_t i = 0; i < numNodes; ++i)
                trees[n]->children[i] = trees[childrenBuffer[i]];
        }
        //returns is eof
        bool eatSpace() {
            for (;;) {
                char c = f.get();
                if (isEof(c)) return true;
                if (c == '\n') {
                    ++line;
                } else if (!isspace(c)) {
                    f.unget();
                    break;
                }
            }
            return false;
        }
        /*void debugPrintC(int n=3) {
            printf("dbg: '");
        
            for (int i = 0;i<n; ++i)
                printf("%c", f.get());
            printf("'\n");
            for (int i = 0;i<n; ++i)
                f.unget();
        }*/

        bool checkChar(char c) {
            eatSpace();

            if (f.get() != c) {
                f.unget();
                return false;
            }
            return true;
        }
        void enforceKeyword(const char* msg) {
            eatSpace();
            const char* p = msg;
            while (*p) {
                if (f.get() != *p++) {
                    //debugPrintC(3);
                    expectedErr(msg);
                    ///return false;
                }
            }
        }
        template<typename T>
        T enforceVal(const char* msg) {
            eatSpace();
            T res {};
            if (!(f >> res)) syntaxError(msg);
            return res;
        }
        void expectedErr(const char* msg) {
            std::cerr << "Syntax error on line " << line << ": "
                      << "Expected '" << msg << "', got"
                      << "'" << char(f.get()) << "'" << "\n";
            throw std::runtime_error("syntax error");
        }
        void syntaxError(const char* msg) {
            std::cerr << "Syntax error on line " << line << ": "
                      << msg << "\n";
            throw std::runtime_error("syntax error");
        }
        constexpr static bool isEof(char c) {
            return c == std::char_traits<char>::eof();
        }

    };

    static Tree* fromFile(const char* path) {
        Parser p(path);
        return p.res;
    }
    void print(std::ostream& s, size_t indent = 0) const {
        for (size_t i = 0; i < indent*2; ++i) std::cout << ' ';

        std::cout << n << (children.size() == 0 ? "\n" :":\n");
        for (auto c : children)
            c->print(s, indent+1);
    }

    friend std::ostream& operator<<(std::ostream& s, const Tree* t) {
        t->print(s);
        return s;
    }
};
template<typename F>
void visitPreorder(const Tree* t, F visitor) {
    visitor(t);
    for (auto c: t->children) visitPreorder(c, visitor);
}

template<typename F>
void visitBreadthFirst(const Tree* t, F visitor) {
    Vector<const Tree*> q = {t};
    while (!q.empty()) {
        const Tree* curr = q.front();
        q.pop_front();
        visitor(curr);
        for (auto c : curr->children) {
            q.push_back(c);
        }
    }
}
//returns tree height
size_t printLevels(const Tree* t, size_t level = 0) {
    std::cout << t->n << ": " << level << "\n";
    size_t max = level;
    for (auto c: t->children) {
        size_t val = printLevels(c, level+1);
        if (max < val) max = val;
    }
    return max;
}
int main() {
    Tree* t = Tree::fromFile("tree.txt");
    std::cout <<t;
    std::cout << "Preorder:\n";
    auto visitor = [](const Tree* t) { std::cout << t->n << " "; };
    visitPreorder(t, visitor);
    std::cout << "\nBreadth First:\n";
    visitBreadthFirst(t, visitor);
    std::cout << "\n\n";

    size_t height = printLevels(t);
    std::cout << "Tree height: " << height << "\n";

    return 0;
}
