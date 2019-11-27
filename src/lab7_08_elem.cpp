#include "utils.h"
#include "graphAdjacencyList.h"

#include <iostream>

using Graph = Graph_AdjacencyList;

// A simple path or a simple cycle is a path or cycle that has no
// repeated vertices and consequently no repeated edges.
void printSimplePaths(const Graph& g, PtrRange<Graph::Vertex> verts) {
    for (auto v : g[verts.back()]) {
        //if (verts.front() == v) {
        //*verts.last++ = v;
        //    continue;
        // } else
        if (verts.contains(v)) { continue; }
        auto res = PtrRange<Graph::Vertex>(verts.first, verts.last+1);
        *verts.last = v;
        std::cout << res << "\n";
        printSimplePaths(g, res);
    }
}
void printSimplePaths(const Graph& g) {
    // I assume that a node is not a simple path.
    std::cout << "Simple Paths: \n";
    if (g.edgeCount() == 0) {
        std::cout << "No simple paths found\n";
        return;
    }
    using Vertex = Graph::Vertex;
    Vertex* verts = (Vertex*) alloca(sizeof(Vertex) * g.vertCount());
    for (Vertex v = 0; v < g.vertCount(); ++v) {
        verts[0] = v;
        printSimplePaths(g, PtrRange<Vertex>(verts, verts+1));
    }
}

int main() {
    auto g = Graph::fromFile("nodes.txt");
    std::cout << "g:\n" << g << "\n";
    printSimplePaths(g);
    return 0;
}
