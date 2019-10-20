#include "vectors.h"
#include "utils.h"

#include <iostream>
#include <cmath>

struct Punct2D {
    union {
        struct {
            double x, y;
        };
        double data[2];
    };

    constexpr double* begin() { return data; }
    constexpr const double* begin() const { return data; }

    constexpr double* end() { return data + 2; }
    constexpr const double* end() const { return data + 2; }

    constexpr Punct2D() : x(0), y(0) {}
    constexpr Punct2D(double x, double y) : x(x), y(y) {}

    constexpr double& operator[](size_t i) { return data[i]; }
    constexpr double operator[](size_t i) const { return data[i]; }

    friend std::ostream& operator<<(std::ostream& s, const Punct2D& p) {
        return s << "(" << p.x << ", " << p.y << ")";
    }
    friend std::istream& operator>>(std::istream& s, Punct2D& p) {
        for (auto& v : p) s >> v;
        return s;
    }
    double norm() const;
    double norm2() const;
};

double norm2(const Punct2D& a) {
    return a.x * a.x + a.y * a.y;
}
double norm(const Punct2D& a) {
    return std::sqrt(norm2(a));
}
double Punct2D::norm() const { return ::norm(*this); }
double Punct2D::norm2() const { return ::norm2(*this); }

double distance2(Punct2D a, Punct2D b) {
    return norm2(Punct2D(a.x - b.x, a.y - b.y));
}
double distance(Punct2D a, Punct2D b) {
    return norm(Punct2D(a.x - b.x, a.y - b.y));
}

constexpr double eps = 1e-5;
struct Circle {
    double r;
    Punct2D center;
};
struct Triangle {
    Punct2D a, b, c;
    constexpr Triangle(Punct2D a, Punct2D b, Punct2D c)
        : a(a), b(b), c(c) {}
    constexpr double det() const {
        return a.x * b.y + b.x * c.y + a.y *c.x -
            c.x * b.y - c.y * a.x - a.y * b.x;
    }
    constexpr bool coliniar() const { return det() < eps; }
    constexpr double area() const {
        return std::abs(det()) * 0.5;
    }
    constexpr Circle toCircle() const {
        double S = area();
        if (S < eps) throw std::logic_error("Points are coliniar");
        //double l1 = distance(a, b);
        //double l2 = distance(a, c);
        // double l3 = distance(c, b);
        /*
        Punct2D o{
            norm(a) * (b.y - c.y) + norm(b) * ( norm(c),
                                                0
                                                };

            return Cerc((l1*l2*l3) / (4 * area), o);
        */
        return Circle {};
    }
};

void subpunct_a() {
    Punct2D a, b;
    std::cin >> a>> b;
    std::cout << "a:" << a << "\n";
    std::cout << "b:" << b << "\n";
    std::cout << "d:" << distance(a, b) << "\n";
}
void subpunct_b() {
    int n = readSize("n");
    Punct2D* points = new Punct2D[n];
    Punct2D* end = points + n;
    Punct2D a = read<Punct2D>("A");

    DynamicVector<Punct2D> min(n);
    min[0] = points[0];

    DynamicVector<Punct2D> max(n);
    max[0] = points[0];
    double minDist = distance2(min[0], a);
    double maxDist = minDist;

    for (auto it = points + 1; it < end; ++it) {
        auto d = distance2(a, *it);
        if (d < minDist) {
            minDist = d;
            min.clearAndAdd(*it);
        } else if (d > maxDist) {
            maxDist = d;
            max.clearAndAdd(*it);
        }
    }
    std::cout << "closest: " << min << "; " << distance(min[0], a) << "\n";
    std::cout << "farthest: " << max << "; " << distance(max[0], a) << "\n";


    delete[] points;
}

int main() {
    subpunct_b();

    return 0;
}

/*
void test() {
	try {
		int n = readSize("n");
		Vector2D *v = new Vector2D[n];
		v[0] = Vector2D::read();

		Vector2D max = v[0];
		double maxNorm = max.norm2();
		Vector2D min = v[0];
		double minNorm = maxNorm;

		Vector2D sum = v[0];

		for (int i = 1; i < n; ++i) {
			v[i] = Vector2D::read();
			auto& vec = v[i];
			sum += vec;
			auto norm = vec.norm2();
			if (norm > maxNorm) {
				maxNorm = norm;
				max = vec;
			}
			else if (norm < minNorm) {
				minNorm = norm;
				min = vec;
			}
		}
		std::cout << "max:" << max << ";" << max.norm() << "\n";
		std::cout << "min:" << min << ";" << min.norm() << "\n";

		std::cout << "sum" << sum << "\n";



		/ *
		Vector2D a = Vector2D::read();// { 3, 4, };

		Vector2D b = Vector2D::read();// { 3, 4, };
		//	Vector2D b{ 2, 3,};
		std::cout << "a:  " << a << "\n";
		std::cout << "b:  " << b << "\n";
		std::cout << "a+b:" << a + b << "\n";
		std::cout << "2*b:" << 2* b << "\n";
		std::cout << "-a:" << -a << "\n";
		std::cout << "a-b:" << a - b << "\n";
		std::cout << "a-a:" << a - a << "\n";
		std::cout << "<a, b>:" << dot(a, b) << "\n";
		std::cout << "<a, a>:" << dot(a, a) << "\n
		* /
		//std::cout << "||a||:" << norm(a) << "\n";
	}
	catch (std::exception& e) {
		std::cerr << "Error:" << e.what();
	}
}
using Punct2D = Vector2D;
*/
