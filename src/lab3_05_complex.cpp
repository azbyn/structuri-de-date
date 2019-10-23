#include "utils.h"

#include <iostream>
#include <utility>
#include <cmath>

struct Complex {
    double re, im;

    constexpr Complex() : re(0), im(0) {}
    constexpr Complex(double re, double im) : re(re), im(im) {}
    constexpr Complex(double re) : re(re), im(0) {} //leaving it implicit is desired


    friend std::ostream& operator<<(std::ostream& s, const Complex& c) {
        return s << c.re << std::showpos << c.im << "i" << std::noshowpos;
    }

    friend std::istream& operator>>(std::istream& s, Complex& c) {
        return s >> c.re >> c.im;
    }
    constexpr double norm2() const {
        return re * re + im * im;
    }
    double norm() const {
        return std::sqrt(norm2());
    }

    constexpr Complex conj() const {
        return Complex {re, - im };
    }

    constexpr Complex operator+(Complex rhs) const {
        return Complex {re+ rhs.re, rhs.im + im };
    }

    constexpr Complex operator-(Complex rhs) const {
        return Complex {re- rhs.re, rhs.im - im };
    }

    constexpr Complex operator-() const {
        return Complex {-re, -im };
    }

    constexpr Complex operator+() const {
        return *this;
    }

    constexpr Complex& operator+=(Complex rhs) {
        this->im += rhs.im;
        this->re += rhs.re;
        return *this;
    }

    constexpr Complex operator*(double rhs) const {
        return Complex {rhs * re,  im * rhs };
    }

    constexpr friend Complex operator*(double lhs, Complex rhs) {
        return Complex {rhs.re*lhs,  rhs.im * lhs };
    }

    constexpr Complex operator*(Complex rhs) const {
        return Complex {rhs.re * re -rhs.im * im,  im * rhs.re + re * rhs.im };
    }

    constexpr Complex operator/(Complex rhs) const {
        return (1.0/ rhs.norm2()) * Complex { rhs.re * re + rhs.im * im,
                im * rhs.re - re * rhs.im };
    }
};
void test() {
    Complex a {1, 2 };
    Complex b {2, -3 };
    Complex c = 2;
    Complex d {0, 1};
    std::cout << a << " "<< b << "\n";
    std::cout << c << " "<< d << "\n";
    std::cout << "a+b " << a + b << "\n";
    std::cout << "a-b " << a - b << "\n";
    std::cout << "a*b " << a * b << "\n";
    std::cout << "a/b " << a / b << "\n";
    std::cout << "conj a " << a.conj() << "\n";
    std::cout << "norm a " << a.norm() << "\n";
}

int main() {
    test();
    int n = readSize("n");
    Complex *p = new Complex[n];
    Complex *end = p +n;
    std::cin >> *p;

    Complex sum = *p;
    Complex* maxIt = p;
    double maxNorm2 = maxIt->norm2();

    for (auto it = p+1; it < end; ++it) {
        std::cin >> *it;
        sum += *it;
        auto norm2 = it->norm2();
        if (norm2 > maxNorm2) {
            maxIt = it;
            maxNorm2 = norm2;
        }
    }

    std::cout << "sum:" << sum << "\n";
    std::cout << "max:" << *maxIt << ", norm:" << maxIt->norm() << "\n";
}
