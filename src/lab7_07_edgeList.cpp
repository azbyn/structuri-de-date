#include "utils.h"
#include "list.h"

#include <iostream>
#include <fstream>

struct Graph_EdgeList {
    using Vertex = unsigned;
    struct Edge {
        Vertex a, b;
        constexpr Edge(Vertex a, Vertex b) :a(a), b(b) {}
        friend std::ostream& operator<< (std::ostream& s, Edge e) {
            return s << "(" << e.a << ", "<< e.b << ")";
        }

        friend std::istream& operator>> (std::istream& s, Edge& e) {
            return s >> e.a >>e.b;
        }
        constexpr bool operator ==(Edge rhs) const {
            return (rhs.a == a && rhs.b == b) ||
                (rhs.b == a && rhs.a == b);
        }
    };
    List<Edge> edges;
    size_t nodeSize;

    template <typename Path>
    static Graph_EdgeList fromFile(Path path) {
        std::ifstream f(path);
        size_t sz = 0;
        f >> sz;
        Graph_EdgeList g { {}, sz };
        Vertex a, b;
        while (f >> a >> b) { g.addEdge(a, b); }
        return g;
    }
    void addEdge(Vertex a, Vertex b) { edges.emplace_front(a, b); }
    void addEdge(Edge e) { edges.push_front(e); }
    
    friend std::ostream& operator<<(std::ostream& s,
                                    const Graph_EdgeList& g) {
        return s << g.edges;
    }
    constexpr bool hasEdge(Edge e) const {
        return edges.findElem(e)!= nullptr;
    }

    constexpr bool hasEdge(Vertex a, Vertex b) const {
        return hasEdge({a, b});
    }
};

int main() {
    auto g = Graph_EdgeList::fromFile("nodes.txt");
    std::cout << "g:" << g << "\n";
    std::cout << "has: "<< g.hasEdge(3, 2);
    return 0;
}
