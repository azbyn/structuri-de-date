#include "utils.h"
#include "ptrRange.h"

#include <iostream>
#include <limits>
#include <utility>

struct WeightedDirGraph_Mat {
    using Graph = WeightedDirGraph_Mat;
    using Vertex = unsigned;
    using Weight = double;

    using Matrix = ManagedMatrixPtrRange<Weight>;
    static_assert(std::numeric_limits<Weight>::has_infinity);
    static constexpr Weight inf = std::numeric_limits<Weight>::infinity();
    static_assert(inf + 1 == inf);
    static_assert(inf - 1 == inf);
    static_assert(1 < inf);

    Matrix mat;
    constexpr size_t size() const { return mat.cols; }
    WeightedDirGraph_Mat(size_t size): mat(size, size) {
        for (size_t i = 0; i < size*size; ++i) {
            mat[i] = inf;
        }
        for (size_t i = 0; i < size; ++i)
            addArc(i, i, 0);
    }

    static WeightedDirGraph_Mat fromFile(const char* path) {
        std::ifstream f(path);
        size_t sz = 0;
        f >> sz;
        Graph g(sz);
        Vertex a, b;
        Weight w;

        while (f >> a >> b >> w) { g.addArc(a, b, w); }
        return g;
    }

    friend std::ostream& operator<< (std::ostream& s, const Graph& g) {
        return s << g.mat;
    }

    constexpr void addArc(Vertex a, Vertex b, Weight w) {
        mat(a, b) = w;
    }
    constexpr Weight arcWeight(Vertex a, Vertex b) const {
        return mat(a, b);
    }
};
