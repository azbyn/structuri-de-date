#include "utils.h"

#include "vector.h"
#include "matrix.h"

#include <iostream>
#include <cmath>

constexpr double eps = 1e-7;

Vec& mul(const Mat& m, const Vec& v, Vec& res) {
    assert(v.begin() != res.begin(), "Can't multiply inplace");
    assert(v.size() == size_t(m.n), "Sizes don't match");
    res.setSize(m.m);


    for (size_t i = 0; i < res.size(); ++i) {
        res[i] = 0;
        for (size_t k = 0; k < v.size(); ++k)
            res[i] += m.at(i, k) * v[k];
    }
    return res;
}

// A * X = b
struct System {
    Mat A;
    Vec b;

    System(int n, int m) : A(n, m), b(n) {}

    System(std::initializer_list<std::initializer_list<double>> A,
           std::initializer_list<double> b) : A(A), b(b) {
           assert(std::size_t(this->A.m) == this->b.size(),
                  "sizes don't match");
    }

    friend std::ostream& operator<<(std::ostream& s, const System& sys) {
        s << "System " << sys.A.m << "x" << sys.A.n << ": \n";
        auto& A = sys.A;
        for (int i = 0; i < A.m; ++i) {
            s << "{";
            for (int j = 0; j < A.n; ++j) {
                //showpos shows a '+' in front of positive numbers
                if (std::abs(A.at(i, j)) > eps)
                    s << std::showpos << A.at(i, j)
                      << std::noshowpos << "*x" << (j+1) << " ";
            }
            s<< "= " << sys.b[i] << "\n";
        }

        return s;
    }

    // L_i += f * L_j
    void addLines(int i, double f, int j) {
        for (int k = 0; k < A.n; ++k) {
            A.at(i, k) += f * A.at(j, k);
        }
        b[i] += f * b[j];
    }

    // L_i *= f
    void multiplyLine(int i, double f) {
        for (int k = 0; k < A.n; ++k) {
            A.at(i, k) *= f;
        }
        b[i] *= f;
    }

    Vec solveTriangulated() {
        for (int i = A.m-1; i > 0; --i) {
            addLines(i-1, -A.at(i-1, i), i);
            multiplyLine(i, 1 / A.at(i, i));
        }
        return b;
    }

    bool checkSolution(const Vec& x) const {
        Vec r;
        mul(A, x, r);
        sub(b, r, r);
        for (auto& v : r) {
            if (std::abs(v) > eps) return false;
        }
        return true;
    }

    static Vec solveCustom(int n) {
        System s(n, n);
        for (auto& v : s.A) v = 0;
        s.A.at(0,0) = 2;
        s.A.at(0,1) = -1;
        s.b[0] = 1;
        for (int i = 1; i < n - 1; ++i) {
            s.b[i] = 1;

            s.A.at(i,i-1) = -1;
            s.A.at(i,i) = 2;
            s.A.at(i,i+1) = -1;
        }
        s.b[n-1] = 1;
        s.A.at(n-1, n-2) = -1;
        s.A.at(n-1, n-1) = 2;
        s.customTriangulate();
        return s.solveTriangulated();
    }
    void customTriangulate() {
        multiplyLine(0, 1 / A.at(0, 0));
        for (int i = 1; i < A.m; ++i) {
            addLines(i, 1, i-1);
            multiplyLine(i, 1 / A.at(i, i));
        }
    }
};

int main() {
    try {
        int n = readSize("n", 2, 51);
        std::cout << "x = "<< System::solveCustom(n) << "\n";
    } catch (std::exception& e) {
        std::cerr << "Error" << e.what() << "\n";
        return 1;
    }
    return 0;
}
