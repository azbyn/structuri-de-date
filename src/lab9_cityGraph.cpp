#include "weightedDirGraphMat.h"
#include <iostream>
#include <iomanip>

using Graph = WeightedDirGraph_Mat;
using Vertex = Graph::Vertex;
using Weight = Graph::Weight;

constexpr Vertex invalidVert = -1;

template<typename T>
using Matrix = ManagedMatrixPtrRange<T>;

struct DijkstraRes {
    ManagedPtrRange<Weight> dist;
    ManagedPtrRange<Vertex> prev;
    Vertex from;

    bool printPath(Vertex u) {
        StaticVector<Vertex> s(prev.size());
        while (u != from) {
            if (prev[u] == invalidVert) return false;
            s.push_back(u);
            u = prev[u];
        }
        std::cout << from<< " ";
        for (auto it = s.last-1; it > s.first -1; --it) {
            std::cout << *it << " ";
        }
        std::cout << "\n";
        return true;
    }
};
auto dijkstra(const Graph& g, Vertex from) {
    DijkstraRes res = { {g.size()}, { g.size()}, from};
    StaticVector<Vertex> q(g.size(), g.size());
    for (Vertex v = 0; v < g.size(); ++v) {
        res.dist[v] = Graph::inf;
        res.prev[v] = invalidVert;
        q[v] = v;
    }
    res.dist[from] = 0;

    while (q.size() != 0) {
        auto it = q.min();
        auto u = *it;
        q.remove(it);
        for (Vertex v = 0; v < g.size(); ++v) {
            if (v == u) continue;
            auto val = g.mat(u, v);
            if (val != Graph::inf) {
                auto alt = res.dist[u] + val;
                if (alt < res.dist[v]) {
                    res.dist[v] = alt;
                    res.prev[v] = u;
                }
            }
        }
    }
    return res;
}

struct RoyFloydRes {
    Matrix<Weight> dist;
    Matrix<Vertex> next;

    bool printPath(Vertex u, Vertex v) {
        if (next(u, v) == invalidVert) {
            std::cout << "No path\n";
            return false;
        }
        while (u != v) {
            std::cout << u << " ";
            u = next(u, v);
        }
        std::cout << v << "\n";
        return true;
    }
};

RoyFloydRes royFloyd(const Graph& g) {
    auto n = g.size();
    RoyFloydRes res = {g.mat, {n,n} };

    for (Vertex u = 0; u < n; ++u) {
        for (Vertex v = 0; v < n; ++v) {
            res.next(u, v) = res.dist(u, v) != Graph::inf? v : invalidVert;
        }
    }
    for (Vertex k = 0; k < n;++k)
        for (Vertex i = 0; i < n;++i)
            for (Vertex j = 0; j < n; ++j) {
                if (res.dist(i, j) > res.dist(i, k) + res.dist(k, j)) {
                    res.dist(i, j) = res.dist(i, k) + res.dist(k, j);
                    res.next(i, j) = res.next(i, k);
                }
            }
    return res;
}

int main() {
    Graph g = Graph::fromFile("nodes.txt");
    std::cout << std::setw(3) << g;

    auto s = read<Vertex>("Start");
    auto e = read<Vertex>("End");
    auto dij = dijkstra(g, s);
    dij.printPath(e);

    std::cout << "\nR-F:\n";

    auto res = royFloyd(g);
    std::cout << std::setw(3) << res.dist;
    std::cout << "Next:\n" << res.next << "\n";
    res.printPath(1, 2);

    auto res2 = dijkstra(g, 1);

    res2.printPath(2);
    return 0;
}
