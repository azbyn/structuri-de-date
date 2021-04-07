#pragma once
#include "utils.h"

#include <iostream>
#include <cmath>
#include <cstring>
#include <utility>

struct Mat {
    int m, n;
    double *data;
    Mat() : m(0), n(0), data(nullptr) {}
    Mat(int m, int n) : m(m), n(n) { data = new double[m*n]; }
    //template<int m, int n>
    Mat(std::initializer_list<std::initializer_list<double>> list) {
        getSize(list, m, n);
        data = new double[m*n];

        auto it = begin();
        for (const auto& l : list) {
            for (const auto& v : l) *(it++) = v;
        }
    }
    Mat(Mat&& rhs) noexcept :
        m(rhs.m), n(rhs.n),
        data(std::exchange(rhs.data, nullptr)) {
        //rhs.n = rhs.m = 0;
    }

    Mat(const Mat& rhs) : Mat(rhs.m, rhs.n) {
        std::memcpy(data, rhs.data, sizeof(double) *m*n);
    }
    Mat& operator=(const Mat& rhs) {
        setSize(rhs.m, rhs.n);
        std::memcpy(data, rhs.data, sizeof(double) *m*n);
        return *this;
    }
    Mat& operator=(Mat&& rhs) noexcept {
        this->~Mat();
        m = rhs.m;
        n = rhs.n;
        data = std::exchange(rhs.data, nullptr);

        return *this;
    }

    ~Mat() {
        delete[] data;
    }
    double& at(int i, int j) {
        assert(i < m && j < n, "out of range");
        return data[i *n +j];
    }
    double at(int i, int j) const { return data[i * n + j]; }
    double* begin() { return data; }
    const double* begin() const { return data; }
    double* end() { return data + m*n; }
    const double* end() const { return data + m*n; }

    void setSize(int m, int n) {
        if (this->m == m && this->n == n) return;
        this->~Mat();
        new (this) Mat(m, n);
    }

    static Mat read() {
        Mat res(readSize("m"), readSize("n"));
        for (auto& i : res)
            std::cin >> i;
        return res;
    }
    void print(const char* name) const {
        std::cout << name << " = Mat " << m << "x" << n << "{\n";

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                std::cout << at(i, j) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "}\n";
    }

    friend std::ostream& operator<<(std::ostream& s, Mat& m) {
        s << "Mat " << m.m << "x" << m.n << "{\n";
        for (int i = 0; i < m.m; ++i) {
            for (int j = 0; j < m.n; ++j) {
                s << m.at(i, j) << " ";
            }
            s << "\n";
        }
        return s<< "}\n";
    }

    double norm1() const { return normImpl<Type::Row>(n, m); }
    double normInf() const { return normImpl<Type::Col>(m, n); }
    double normF() const {
        double res = 0;
        for (int i = 0; i < m; ++i)
            for (int j = 0; j < n; ++j)
                res += at(i, j) * at(i, j);

        return std::sqrt(res);
    }
    bool isStrictlyRowDiagonallyDominant() const {
        return isStrictlyDiagonallyDominantImpl<Type::Row>();
    }

    bool isStrictlyColDiagonallyDominant() const {
        return isStrictlyDiagonallyDominantImpl<Type::Col>();
    }

private:

    enum class Type { Row, Col };
    template<Type type>
    bool isStrictlyDiagonallyDominantImpl() const {
        assert(m == n, "Matrix must be square");
        for (int i = 0; i < m; ++i) {
            double val = std::abs(at(i, i));
            double sum = -val;
            for (int j = 0; j < m; ++j) {
                sum += std::abs(type == Type::Col? at(j, i): at(i, j));
            }
            if (sum >= val) {
                std::cout << "(" << sum << "," << val << ")";
                return false;
            }
        }
        return true;
    }
    template<Type type>
    double normImpl(int sz1, int sz2) const {
        double max = -1;
        for (int j = 0; j < sz1; ++j) {
            double x = 0;
            for (int i = 0; i < sz2; ++i) {
                x += std::abs(type == Type::Col? at(j, i) : at(i, j));
            }
            if (x > max) max = x;
        }
        return max;
    }
};

Mat& add(const Mat& a, const Mat& b, Mat& res) {
    assert(a.m == b.m && a.n == b.n, "Sizes don't match, can't add");
    res.setSize(a.m, a.n);
    for (int i = 0; i < a.m; ++i)
        for (int j = 0; j < a.n; ++j)
            res.at(i, j) = a.at(i, j) + b.at(i, j);
    return res;
}

Mat& mul(double a, const Mat& b, Mat& res) {
    res.setSize(b.m, b.n);

    for (int i = 0; i < res.m; ++i)
        for (int j = 0; j < res.n; ++j)
            res.at(i, j) = a * b.at(i, j);
    return res;
}

Mat& neg(const Mat& a, Mat& res) {
    return mul(-1, a, res);
}
Mat& sub(const Mat& a, const Mat& b, Mat& res) {
    return add(a, neg(b, res), res);
}
Mat& mul(const Mat& a, const Mat& b, Mat& res) {
    assert(a.n == b.m, "Sizes don't match, can't multiply");
    res.setSize(a.m, b.n);

    for (int i = 0; i < res.m; ++i)
        for (int j = 0; j < res.n; ++j) {
            res.at(i, j) = 0;
            for (int k = 0; k < a.n; ++k)
                res.at(i, j) += a.at(i, k) * b.at(k, j);
        }
    return res;
}

Mat& trans(const Mat& a, Mat& res) {
    assert(a.data != res.data, "Can't transpose inplace");
    res.setSize(a.n, a.m);

    for (int i = 0; i < res.m; ++i)
        for (int j = 0; j < res.n; ++j)
            res.at(i, j) = a.at(j, i);
    return res;
}
