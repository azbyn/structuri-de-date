#include <iostream>
#include <cmath>

using namespace std;
#define MAX_SZ 8

// const is optional, through recommended
double norm1(const int A[MAX_SZ][MAX_SZ], int m, int n) {
    double max = -1;
    for (int j = 0; j < m; ++j) {
        double x = 0;
        for (int i = 0; i < n; ++i) {
            x += abs(A[i][j]);
        }
        if (x > max) {
            max = x;
        }
    }
    return max;
}

double norm2(const int A[MAX_SZ][MAX_SZ], int m, int n) {
    double max = -1;
    for (int j = 0; j < n; ++j) {
        double x = 0;
        for (int i = 0; i < m; ++i)
            x += abs(A[j][i]);
        if (x > max) max = x;
    }
    return max;
}

double normF(const int A[MAX_SZ][MAX_SZ], int m, int n) {
    double res = 0;
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j)
            res += A[i][j] * A[i][j];

    return sqrt(res);
}
int do_something_undefined() {
    return 1 / 0;
}

bool isStrictlyRowDiagonallyDominant(const int A[MAX_SZ][MAX_SZ],
                                     int m, int n) {
    if (m != n) {
        cout << "launching nuclear missile because"
             << "matrix is not square\n";
        do_something_undefined();
    }

    for (int i = 0; i < m; ++i) {
        int val = std::abs(A[i][i]);
        int sum = -val;
        for (int j = 0; j < m; ++j)
            sum += abs(A[i][j]);
        if (sum >= val) return false;
    }
    return true;
}
//implement this, natürlich
void transpose(const int A[MAX_SZ][MAX_SZ], int res[MAX_SZ][MAX_SZ],
               int m, int n);

//not the most efficient, but good enough for demonstration purposes
bool isStrictlyColDiagonallyDominant(const int A[MAX_SZ][MAX_SZ],
                                     int m, int n) {
    int res[MAX_SZ][MAX_SZ];
    transpose(A, res, m, n);
    return isStrictlyRowDiagonallyDominant(res, n, m);
}
