#include <iostream>
#include <cmath>

constexpr int MAX_SZ = 8;
int readSize(const char* name) {
    int res;
    do {
        std::cout << name << ": ";
        std::cin >> res;
    } while (res <= 0 || res >= MAX_SZ);
    return res;
}

void assert(bool cond, const char* msg) {
    if (!cond) throw std::logic_error(msg);
}

struct Mat {
    double data[MAX_SZ][MAX_SZ] {};
    int m, n;

    constexpr Mat() : m(0), n(0) {}
    constexpr Mat(int m, int n) : m(m), n(n) {}
    constexpr Mat(std::initializer_list<std::initializer_list<double>> list) : Mat(0, -1) {
        for (const auto& l : list) {
            int i = 0;
            for (const auto& v : l) {
                data[m][i++] = v;
            }
            if (n != -1 && i != n) {
                throw std::logic_error("Matrix should have fixed line size");
            }
            ++m;
            n = i;
        }
    }

    double& at(int i, int j) { return data[i][j]; }
    double at(int i, int j) const { return data[i][j]; }

    void setSize(int m, int n) {
        this->m = m;
        this->n = n;
    }

    static Mat read() {
        Mat res(readSize("m"), readSize("n"));

        for (int i = 0; i < res.m; ++i)
            for (int j = 0; j < res.n; ++j)
                std::cin >> res.data[i][j];
        return res;
    }
    void print(const char* name) const {
        std::cout << name << " = Mat " << m << "x" << n << "{\n";

        for (int i = 0; i < m; ++i) {
            for (int j = 0; j < n; ++j) {
                std::cout << data[i][j] << " ";
            }
            std::cout << "\n";
        }
        std::cout << "}\n";
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
            int val = std::abs(at(i, i));
            int sum = -val;
            for (int j = 0; j < m; ++j) {
                sum += std::abs((type == Type::Col) ? at(j, i) : at(i, j));
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
                x += std::abs((type == Type::Col) ? at(j, i) : at(i, j));
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
    assert(a.data != res.data, "Can't calculate the transpose inplace");
    res.setSize(a.n, a.m);

    for (int i = 0; i < res.m; ++i)
        for (int j = 0; j < res.n; ++j)
            res.at(i, j) = a.at(j, i);
    return res;
}


int main() {
    const Mat a = {
        {4, -2, 1, 3},
        {1, -5, 2, 4},
        {-1, 1, 3, 15},
        };
    a.print("a");
    /*Mat b = {

        {4, -2, 1},
        {1, -5, 0},
        {1, 2, 3},
    };
    Mat res, res2;
    a.print("a");
    b.print("b");
    mul(a, b, res).print("mul");
    mul(b, a, res).print("mul");
    add(a, b, res).print("add");
    sub(a, b, res).print("sub");
    mul(2, b, res).print("mul");
    trans(mul(a, b, res2), res).print("trans");
    */
    std::cout << a.norm1() << "\n";
    std::cout << a.normInf() << "\n";
    /*
    std::cout <<"normInf\n";
    std::cout << a.normInf() << "\n";
    std::cout << b.normInf() << "\n";

    std::cout <<"normF\n";
    std::cout << a.normF() << "\n";
    std::cout << b.normF() << "\n";

    std::cout <<"iSCDD: "<< a.isStrictlyColDiagonallyDominant() << "\n";
    std::cout <<"iSCDD :"<< b.isStrictlyColDiagonallyDominant() << "\n";

    std::cout <<"iSRDD: "<< a.isStrictlyRowDiagonallyDominant() << "\n";
    std::cout <<"iSRDD :"<< b.isStrictlyRowDiagonallyDominant() << "\n";
    */
    return 0;
}
