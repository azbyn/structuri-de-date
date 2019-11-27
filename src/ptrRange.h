#pragma once
#include <iostream>
#include <utility>

template<typename T>
struct PtrRange {
    T* first;
    T* last;
    constexpr PtrRange(T* first, T* last)
        : first(first), last(last){}

    constexpr PtrRange(T* first, size_t sz)
        : first(first), last(first+sz){}
    constexpr T* begin() { return first; }
    constexpr T* end() { return last; }

    constexpr const T* begin() const { return first; }
    constexpr const T* end() const { return last; }

    constexpr T& front() { return *first; }
    constexpr T& back() { return *(last-1); }
    constexpr const T& front() const { return *first; }
    constexpr const T& back() const { return *(last-1); }


    friend std::ostream& operator<<(std::ostream& s, PtrRange v) {
        s << "(";
        T* it = v.first;
        for (T* end = v.last - 1; it < end; ++it)
            s << *it << ", ";

        if (it < v.last) s << *it;

        return s << ")";
    }
    constexpr const T& operator[](size_t i) const { return first[i]; }
    constexpr T& operator[](size_t i) { return first[i]; }

    constexpr size_t size() const { return last - first; }

    constexpr T* find(const T& val) {
        for (auto it = first; it != last; ++it) {
            if (*it == val) return it;
        }
        return nullptr;
    }

    // P is a predicate on T
    template<typename P>
    constexpr T* find_if(P p) {
        for (auto it = first; it != last; ++it) {
            if (p(it->val)) return it;
        }
        return nullptr;
    }

    template<typename P>
    constexpr T* find_neighbouring_if(P p) {
        for (auto it = first; it != (last-1); ++it) {
            if (p(it->val, (it+1)->val)) return it;
        }
        return nullptr;
    }

    template<typename P>
    constexpr T* find_neighbouring(const T& a, const T& b) {
        for (auto it = first; it != (last-1); ++it) {
            if (*it == a && *(it+1) == b) return it;
        }
        return nullptr;
    }
    constexpr bool contains(const T& val) const { return find(val) != nullptr; }

    template<typename P>
    constexpr T* contains_if(P p) const { return find(p) != nullptr; }

    template<typename P>
    constexpr T* contains_neighbouring_if(P p) const {
        return find_neighbouring_if(p) != nullptr;
    }
};

template<typename T>
struct ManagedPtrRange : public PtrRange<T> {
    ManagedPtrRange(size_t sz)
        : PtrRange<T>(new T[sz], sz) {}
    ~ManagedPtrRange() { delete[] this->first; }

    ManagedPtrRange(const ManagedPtrRange&) = delete;
    ManagedPtrRange(ManagedPtrRange&& rhs)
        : PtrRange<T>::first(std::exchange(rhs.first, nullptr)),
          PtrRange<T>::last(std::exchange(rhs.last, nullptr)) {}
    ManagedPtrRange& operator=(const ManagedPtrRange&) = delete;
    ManagedPtrRange& operator=(ManagedPtrRange&& rhs) {
        this->~ManagedPtrRange();
        this->first = std::exchange(rhs.fist, nullptr);
        this->last = std::exchange(rhs.last, nullptr);
        return *this;
    }

    /*
    friend std::ostream& operator<<(std::ostream& s, const ManagedPtrRange& v) {
        s << "(";
        T* it = v.first;
        for (T* end = v.last - 1; it < end; ++it)
            s << *it << ", ";

        if (it < v.last) s << *it;

        return s << ")";
    }*/
};
