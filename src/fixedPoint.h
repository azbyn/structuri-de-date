#pragma once
#include "utils.h"
#include <iostream>

constexpr char decimalSeparator = '.';

constexpr long pow(long b, long e) {
    long res = 1;
    while (--e >= 0) res *= b;
    return res;
}
template<size_t decimals = 2>
class FixedPoint {
    static_assert(decimals < 18);
    static constexpr long factor = pow(10, decimals);
    int64_t val;
    using FP = FixedPoint<decimals>;

public:
    static constexpr FP make(int64_t v) {
        FP r;
        r.val = v;
        return r;
    }
    constexpr FixedPoint(long double v) : val(v * factor) {}
    constexpr FixedPoint(double v) : val(v * factor) {}
    constexpr FixedPoint(float v) : val(v * factor) {}
    constexpr FixedPoint(long v) : val(v * factor) {}
    constexpr FixedPoint(int v) : val(v * factor) {}
    constexpr FixedPoint() : val(0) {}

    constexpr FP operator+(FP rhs) const { return make(val + rhs.val); }
    constexpr FP operator-(FP rhs) const { return make(val - rhs.val); }
    constexpr FP operator+() const { return *this; }
    constexpr FP operator-() const { return make(-val); }
    constexpr FP operator*(FP rhs) const {
        return make((val * rhs.val) / factor);
    }
    constexpr FP operator/(FP rhs) const {
        return make((val * factor) / rhs.val);
    }

    constexpr FP& operator+=(FP rhs) { return *this = *this + rhs; }
    constexpr FP& operator-=(FP rhs) { return *this = *this - rhs; }
    constexpr FP& operator/=(FP rhs) { return *this = *this / rhs; }
    constexpr FP& operator*=(FP rhs) { return *this = *this * rhs; }

    constexpr bool operator< (FP rhs) const { return val < rhs.val; }
    constexpr bool operator> (FP rhs) const { return val > rhs.val; }
    constexpr bool operator<=(FP rhs) const { return val <= rhs.val; }
    constexpr bool operator>=(FP rhs) const { return val >= rhs.val; }

    constexpr bool operator==(FP rhs) const { return val == rhs.val; }
    constexpr bool operator!=(FP rhs) const { return val != rhs.val; }
    constexpr size_t textLen(bool showSign = false) const {
        size_t baseline = 1 + decimals + showSign; // 1 for the dot
        if (val < 0) return make(-val).textLen(true);
        // a int64_t can only store 19 digits
        if (val < factor * 1) return baseline + 0;
        if (val < factor * 10) return baseline + 1;
        if constexpr (decimals <= 16)
        if (val < factor * 100) return baseline + 2;
        if constexpr (decimals <= 15)
        if (val < factor * 1000) return baseline + 3;
        if constexpr (decimals <= 14)
        if (val < factor * 10000) return baseline + 4;
        if constexpr (decimals <= 13)
        if (val < factor * 100000) return baseline + 5;
        if constexpr (decimals <= 12)
        if (val < factor * 1000000) return baseline + 6;
        if constexpr (decimals <= 11)
        if (val < factor * 10000000) return baseline + 7;
        if constexpr (decimals <= 10)
        if (val < factor * 100000000) return baseline + 8;
        if constexpr (decimals <= 9)
        if (val < factor * 1000000000) return baseline + 9;
        if constexpr (decimals <= 8)
        if (val < factor * 10000000000) return baseline + 10;
        if constexpr (decimals <= 7)
        if (val < factor * 100000000000) return baseline + 11;
        if constexpr (decimals <= 6)
        if (val < factor * 1000000000000) return baseline + 12;
        if constexpr (decimals <= 5)
        if (val < factor * 10000000000000) return baseline + 13;
        if constexpr (decimals <= 4)
        if (val < factor * 100000000000000) return baseline + 14;
        if constexpr (decimals <= 3)
        if (val < factor * 1000000000000000) return baseline + 15;
        if constexpr (decimals <= 2)
        if (val < factor * 10000000000000000) return baseline + 16;
        if constexpr (decimals <= 1)
        if (val < factor * 100000000000000000) return baseline + 17;
        if constexpr (decimals <= 0)
        if (val < factor * 1000000000000000000) return baseline + 18;
        return baseline + (19 - decimals);
    }

    //a buffer of size at most 22 is needed
    //(19 for digits, 1 for the dot, 1 for sign, and 1 for \0)
    static constexpr size_t MaxBuffSize = 22;

    // returns the string length (no \0)
    constexpr size_t toString(char* buf, bool showSign) const {
        auto len = textLen(showSign);
        char* p = buf + len;
        auto i = val;
        if (val < 0) {
            *buf = '-';
            i = -val;
        }
        else if (showSign) *buf = '+';
        *p-- = '\0';
        char* decimalPoint = p - decimals;
        for (; p != decimalPoint; --p) {
            *p = (i % 10) + '0';
            i /= 10;
        }
        *p = decimalSeparator;
        while (i) {
            *(--p) = (i % 10) + '0';
            i /= 10;
        }
        return len;
    }
    static constexpr auto isDigit(char c) {
        return c >= '0' && c <= '9';
    }
    // if the return value is nullptr it means we didn't read anything good
    //else we return a pointer to the end of the read FixedPoint
    static constexpr const char* fromString(const char* str, FP& res) {
        long sign = 1;
        if (*str == '+') ++str;
        else if (*str == '-') { ++str; sign = -1; }
        auto p = str;

        res.val = 0;
        while (isDigit(*p)) {
            res.val *= 10;
            res.val += *p - '0';
            ++p;
        }
        if (*p == '.' || *p == decimalSeparator) {
            ++p;
            if (str == p-1)
                str = p;
            [&] {
                size_t i = 0;
                for (; i < decimals; ++i) {
                    if (!isDigit(*p)) {
                        for (;i < decimals; ++i) res.val *= 10;
                        return;
                    }
                    res.val *= 10;
                    res.val += *p - '0';
                    ++p;
                }
                if (isDigit(*p)) {
                    if (*p - '0' >= 5) ++res.val;
                    ++p;
                }
                while (isDigit(*p)) ++p;
            }();
        } else {
            res.val *= factor;
        }
        if (p == str) { return nullptr; std::cout << "NULL"; }
        res.val *= sign;
        return p;
    }
    static constexpr auto fromString(const char* str) {
        struct {
            FP res;
            const char* str;
        } res;
        res.str = fromString(str, res.res);
        return res;
    }

    friend std::ostream& operator<<(std::ostream& s, FP fp) {
        char buf[MaxBuffSize];
        fp.toString(buf, s.flags() & s.showpos);
        return s << buf;
    }

    friend std::istream& operator>>(std::istream& s, FP& fp) {
        long double ld;
        s >> ld;
        fp = ld;
        return s;
    }
};
