#pragma once

#include "utils.h"
#include "ptrRange.h"
#include "list.h"

#include <iostream>
#include <fstream>
#include <algorithm>//std::count
#include <alloca.h>


struct DirGraph_AdjacencyList {
    using Vertex = unsigned;
private:

    Vertex** adjacencyList;
    Vertex* adjacencyData;

    size_t vertSize;
    size_t arcSize;
    using Graph = DirGraph_AdjacencyList;
public:
    struct Iterator {
        Vertex** p;
        constexpr Iterator(Vertex** p) : p(p) {}

        constexpr Iterator operator+(int x) const { return p + x; }
        constexpr Iterator operator-(int x) const { return p + x; }
        constexpr Iterator& operator++() { ++p; return *this; }
        constexpr Iterator& operator--() { ++p; return *this; }

        constexpr Iterator operator++(int) { auto r = p; ++p; return r; }
        constexpr Iterator operator--(int) { auto r = p; --p; return r; }

        constexpr auto operator*() const {
            return PtrRange<const Vertex>(*p, *(p+1));
        }
        //constexpr auto& operator->() { return ; }
        constexpr bool operator ==(Iterator o) const { return p == o.p; }
        constexpr bool operator !=(Iterator o) const { return p != o.p; }
    };
    constexpr auto begin() const { return Iterator(adjacencyList); }
    constexpr auto end() const { return Iterator(adjacencyList+vertSize); }

    constexpr PtrRange<const Vertex> operator[](Vertex v) const {
        return *(begin() + v);
    }
    size_t vertCount() const { return vertSize; }
    size_t arcCount() const { return arcSize; }

private:
    DirGraph_AdjacencyList(Vertex** adjacencyList, Vertex* adjacencyData,
                        size_t vertSize, size_t edgeSize)
        : adjacencyList(adjacencyList), adjacencyData(adjacencyData),
          vertSize(vertSize), arcSize(edgeSize) {
        adjacencyData = new Vertex[edgeSize];
    }
public:
    DirGraph_AdjacencyList(const Graph&) = delete;
    DirGraph_AdjacencyList(Graph&& rhs)
        : adjacencyList(std::exchange(rhs.adjacencyList, nullptr)),
          adjacencyData(std::exchange(rhs.adjacencyData, nullptr)),
          vertSize(rhs.vertSize), arcSize(rhs.arcSize) {}
    Graph& operator=(const Graph&) = delete;
    Graph& operator=(Graph&& rhs) {
        this->~DirGraph_AdjacencyList();
        vertSize = rhs.vertSize;
        arcSize = rhs.arcSize;
        adjacencyList = std::exchange(rhs.adjacencyList, nullptr);
        adjacencyData = std::exchange(rhs.adjacencyData, nullptr);
        return *this;
    }
    ~DirGraph_AdjacencyList() {
        delete[] adjacencyData;
        delete[] adjacencyList;
    }

    template <typename Path>
    static Graph fromFile(Path path) {
        std::ifstream f(path);
        size_t vertSize = 0;
        f >> vertSize;

        // std::cout << "Verts: " << vertSize << "\n";
        static_assert(sizeof(intptr_t) == sizeof(Vertex*));
        PtrRange<size_t> counts((size_t*)alloca(sizeof(size_t) * vertSize),
                                vertSize);
        for (auto& it:counts) it = 0;

        size_t arcSize = 0;
        Vertex v, w;
        while (f >> v >> w) {
            ++arcSize;
            // std::cout << "L" << v << " " << w << "\n";
            ++counts[v];
        }

        std::cout << "arcSize: " << arcSize << "\n";
        // std::cout << "counts: " << counts << "\n";
        Vertex** list = new Vertex*[vertSize+1];

        for (auto it = counts.begin()+1, end = counts.end()-1;
             it < end; ++it) {
            *it += *(it-1);
        }
        // std::cout << "cumsum: " << counts << "\n";

        PtrRange<Vertex*> countsPtr((Vertex**) counts.begin(),
                                    (Vertex**) counts.end());
        Vertex* data = new Vertex[arcSize*2];
        for (size_t i = vertSize-1; i >  0; --i) {
            countsPtr[i] = list[i] = data + counts[i-1];
        }
        countsPtr[0] = list[0] = data;
        list[vertSize] = data+(arcSize);

        //std::cout << "thing: " << countsPtr << "\n";

        f.clear();
        f.seekg(0);
        f >> vertSize;
        Vertex a, b;
        while (f>> a>> b) {
            *(countsPtr[a]++) = b;
        }
        return Graph(list, data, vertSize, arcSize);
    }

    friend std::ostream& operator<<(std::ostream& s,
                                    const Graph& g) {
        for (size_t i = 0; i < g.vertSize; ++i) {
            s <<  "["<< i << "] = " << g[i] << "\n";
        }
        return s;
    }
    /*
    constexpr bool hasArc(Vertex a, Vertex b) const {
        return (*this)[a].contains(b);
    }*/
};
