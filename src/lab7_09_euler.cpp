#include "utils.h"
#include "graphEdgeList.h"

#include <iostream>

using Graph = Graph_EdgeList;

using Vertex = Graph::Vertex;
using Edge = Graph::Edge;
// has a cycle that visits every edge once
bool isEulerian(Edge* begin,
                Edge* current,
                Edge* end) {
    /*
      edges right of current are unvisited,
      edges on the left form a chain
     */
    if (end - current == 0) {
        if (begin->a == (end-1)->b) {
            std::cout << "Is Eulerian With solution: ";
            for (auto* it = begin; it != end; ++it) {
                std::cout << it->a << " ";
            }
            std::cout << (end-1)->b;
            std::cout << "\n";
            return true;
        }
    }
    auto last = (current-1)->b;
    for (auto* it = current; it != end; ++it) {
        if (it->a == last) ;
        else if (it->b == last) { std::swap(it->b, it->a); }
        else continue;
        std::swap(*current, *it);
        if (isEulerian(begin, current+1, end)) {
            return true;
        }
        std::swap(*current, *it);
    }
    return false;
}
void printIsEulerian(const Graph& g) {
    size_t sz = g.edges.size();
    assert(sz != 0, "size can't be 0");
    //allocate memory on the stack
    Edge *edges = (Edge*) alloca(sizeof(Edge)*sz);
    g.edges.copyTo(edges);
    if (!isEulerian(edges, edges+1, edges + sz)) {
        std::cout << "Graph is not eulerian.\n";
    }
}

int main() {
    auto g = Graph::fromFile("nodes.txt");
    std::cout << "g:\n" << g << "\n";
    printIsEulerian(g);
    return 0;
}
