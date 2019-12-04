#pragma once
#include "utils.h"
#include "ptrRange.h"

#include <iostream>
#include <fstream>
#include <utility>


struct DirGraph_Mat {
    using Graph = DirGraph_Mat;
    using Vertex = unsigned;
    bool* mat;
    size_t size;

    DirGraph_Mat(size_t size) : mat(new bool[size*size]), size(size) {}
    DirGraph_Mat(const Graph&) = delete;
    DirGraph_Mat(Graph&& rhs)
        : mat(std::exchange(rhs.mat, nullptr)), size(rhs.size) {}
    Graph& operator=(const Graph&) = delete;
    Graph& operator=(Graph&& rhs) {
        this->~DirGraph_Mat();
        mat = std::exchange(rhs.mat, nullptr);
        size = rhs.size;
        return *this;
    }
    ~DirGraph_Mat() {
        delete[] mat;
    }

    constexpr auto operator[] (Vertex v) const {
        return PtrRange<const bool>(mat+size*v, size);
    }

    template <typename Path>
    static Graph fromFile(Path path) {
        std::ifstream f(path);
        size_t sz = 0;
        f >> sz;
        Graph g(sz);
        Vertex a, b;
        while (f >> a >> b) { g.addArc(a, b); }
        return g;
    }
    constexpr void addArc(Vertex a, Vertex b) {
        mat[a * size + b] = true;
    }
    constexpr bool hasArc(Vertex a, Vertex b) const {
        return mat[a * size + b];
    }
    void printMatrix() const {
        bool* it = mat;
        bool* end = mat + size*size;
        bool* endl = mat + size;
        for (;endl < end; endl += size) {
            for (;it < endl; ++it)
                std::cout << *it << " ";
            std::cout << "\n";
        }
    }
};
