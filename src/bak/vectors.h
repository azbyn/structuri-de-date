#pragma once
#include <utility>
#include <stdexcept>
#include <iostream>

template<typename T>
class StaticVector {
protected:
    T *_begin, *_end, *_capacity;
public:
    constexpr StaticVector(T* begin, T* end, T* capacity)
        : _begin(begin), _end(end), _capacity(capacity) {}
    constexpr StaticVector(T* begin, T* capacity)
        : StaticVector(begin, begin, capacity) {}

    constexpr void clear() {
        _capacity = _begin;
    }
    constexpr T* begin() { return _begin; }
    constexpr const T* begin() const { return _begin; }

    constexpr T* end() { return _end; }
    constexpr const T* end() const { return _end; }

    constexpr T& operator[](std::size_t i) { return _begin[i]; }
    constexpr const T& operator[](std::size_t i) const { return _begin[i]; }

    constexpr std::size_t size() const { return _end - _begin; }
    constexpr std::size_t capacity() const { return _capacity - _begin; }
    constexpr void add(const T& val) {
        if (_end >= _capacity) throw std::out_of_range("out of range");
        *(_end++) = val;
    }

    constexpr void add(T&& val) {
        if (_end >= _capacity) throw std::out_of_range("out of range");
        *(_end++) = val;
    }
    constexpr void clearAndAdd(const T& val) {
        clear();
        add(val);
    }

    constexpr void clearAndAdd(T&& val) {
        clear();
        add(val);
    }
    friend std::ostream& operator<<(std::ostream& s, const StaticVector<T>& v) {
        s << "[";
        T* it = v._begin;
        for (T* end = v._end - 1; it < end; ++it)
            s << *it << ", ";

        if (it < v._end) s << *it;

        return s << "]";
    }


};
template<typename T>
class DynamicVector : public StaticVector<T> {
public:
    explicit DynamicVector(std::size_t n)
        : StaticVector<T>(new T[n], nullptr) {
        this->_capacity = this->_begin + n;
    }

    ~DynamicVector() { delete this->_begin; }

    DynamicVector(const DynamicVector&) = delete;
    DynamicVector(DynamicVector&& rhs) noexcept :
        StaticVector<T>(
            std::exchange(rhs._begin, nullptr),
            std::exchange(rhs._end, nullptr),
            std::exchange(rhs._capacity, nullptr)) {}
};
