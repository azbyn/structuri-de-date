#include "utils.h"
#include "dirGraphMat.h"
#include "ptrRange.h"
#include "dirGraphAdjacencyList.h"

#include <iostream>

void visitDepthFirst(const DirGraph_Mat& g,
                     DirGraph_Mat::Vertex current,
                     DirGraph_Mat::Vertex* visited) {
    std::cout << current << " ";
    visited[current] = true;
    auto line = g[current];
    for (size_t i = 0; i < g.size; ++i) {
        if (line[i]) {
            if (visited[i]) continue;
            visitDepthFirst(g, i, visited);
        }
    }
}

void visitDepthFirst(const DirGraph_Mat& g) {
    using Graph = DirGraph_Mat;
    using Vertex = Graph::Vertex;

    Vertex* visited = (Vertex*)alloca(sizeof(Vertex) * g.size);
    for (Vertex i = 0; i < g.size; ++i) {
        std::cout << "Nodes from " << i << ":\n";
        for (Vertex j = 0; j <= g.size; ++j) visited[j] = 0;
        visitDepthFirst(g, i, visited);
        std::cout << "\n";
    }
    std::cout << "\n";
}


void visitBreadthFirst(const DirGraph_AdjacencyList& g,
                       DirGraph_AdjacencyList::Vertex current,
                       SimpleQueue<DirGraph_AdjacencyList::Vertex>& queue,
                       DirGraph_AdjacencyList::Vertex* visited) {
    using Graph = DirGraph_AdjacencyList;
    using Vertex = Graph::Vertex;
    queue.push(current);
    visited[current] = true;
    while (!queue.empty()) {
        Vertex i = queue.pop();

        std::cout << i << " ";
        for (auto& v : g[i]) {
            if (visited[v]) continue;
            visited[v] = true;
            queue.push(v);
        }
    }
}
void visitBreadthFirst(const DirGraph_AdjacencyList& g) {
    using Graph = DirGraph_Mat;
    using Vertex = Graph::Vertex;

    Vertex* visited = (Vertex*)alloca(sizeof(Vertex) * g.vertCount());
    Vertex* queueData = (Vertex*)alloca(sizeof(Vertex) * g.arcCount());
    for (Vertex i = 0; i < g.vertCount(); ++i) {
        std::cout << "Nodes from " << i << ":\n";
        SimpleQueue<Vertex> q { queueData, queueData };

        for (Vertex j = 0; j <= g.vertCount(); ++j) visited[j] = 0;
        visitBreadthFirst(g, i, q, visited);
        std::cout << "\n";
    }
    std::cout << "\n";
}

int main() {
    std::cout << "Depth first:\n";
    auto g = DirGraph_Mat::fromFile("nodes.txt");
    g.printMatrix();
    std::cout << "\n";
    visitDepthFirst(g);

    std::cout << "Breadth first:\n";

    auto g2 = DirGraph_AdjacencyList::fromFile("nodes.txt");
    std::cout << g2 << "\n";
    visitBreadthFirst(g2);
    return 0;
}
