#pragma once
#include <iostream>
#include <utility>
#include <iomanip>

template<typename T>
struct PtrRange {
    T* first;
    T* last;
    constexpr PtrRange(T* first, T* last)
        : first(first), last(last){}

    constexpr PtrRange(T* first, size_t sz)
        : first(first), last(first+sz){}

    PtrRange(std::nullptr_t) : first(nullptr), last(nullptr) {}
    constexpr T* begin() { return first; }
    constexpr T* end() { return last; }

    constexpr const T* begin() const { return first; }
    constexpr const T* end() const { return last; }

    constexpr T& front() { return *first; }
    constexpr T& back() { return *(last-1); }
    constexpr const T& front() const { return *first; }
    constexpr const T& back() const { return *(last-1); }

    constexpr operator bool() {
        return first != nullptr;
    }

    friend std::ostream& operator<<(std::ostream& s, PtrRange v) {
        s << "(";
        if (v.first != nullptr) {
            T* it = v.first;
            for (T* end = v.last - 1; it < end; ++it)
                s << *it << ", ";

            if (it < v.last) s << *it;
        }
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

    constexpr T* min() {
        auto smallest = first;
        for (auto it = first+1; it != last; ++it) {
            if (*it < *smallest)
                smallest = it;
        }
        return smallest;
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
    explicit ManagedPtrRange(size_t sz)
        : PtrRange<T>(new T[sz], sz) {}

    ManagedPtrRange(std::nullptr_t n) : PtrRange<T>(n){}
    ManagedPtrRange(std::initializer_list<T> l) : ManagedPtrRange(getSize(l)) {
        T* it = this->first;
        for (auto& v: l) *it++ = v;
    }
    ManagedPtrRange(const ManagedPtrRange&) = delete;
    ManagedPtrRange(ManagedPtrRange&& rhs)
        : PtrRange<T>(std::exchange(rhs.first, nullptr),
                      std::exchange(rhs.last, nullptr)) {}

    ManagedPtrRange& operator=(const ManagedPtrRange&) = delete;
    ManagedPtrRange& operator=(ManagedPtrRange&& rhs) {
        this->~ManagedPtrRange();
        this->first = std::exchange(rhs.first, nullptr);
        this->last = std::exchange(rhs.last, nullptr);
        return *this;
    }
    ~ManagedPtrRange() { delete[] this->first; }
};

template<typename T>
struct MatrixPtrRange : public PtrRange<T> {
    size_t rows, cols;
    MatrixPtrRange(T* first, size_t rows, size_t cols)
        : PtrRange<T>(first, rows*cols), rows(rows), cols(cols) {}
protected:
    MatrixPtrRange(T* first, T* last, size_t rows, size_t cols)
        : PtrRange<T>(first, last), rows(rows), cols(cols) {}
public:
    constexpr T& operator()(size_t i, size_t j) {
        return this->first[cols*i + j];
    }
    constexpr const T& operator()(size_t i, size_t j) const {
        return this->first[cols*i + j];
    }
    friend std::ostream& operator<<(std::ostream& s, MatrixPtrRange p) {
        auto it = p.begin();
        auto end = p.end();
        auto endl = it + p.cols;
        auto w = s.width();
        for (;endl <= end; endl += p.cols) {
            for (;it < endl; ++it)
                s << std::setw(w) << *it << " ";
            s << "\n";
        }
        return s;
    }
};

template<typename T>
struct ManagedMatrixPtrRange : public MatrixPtrRange<T> {
    ManagedMatrixPtrRange(size_t rows, size_t cols)
        : MatrixPtrRange<T>(new T[rows*cols], rows, cols) {}

    ManagedMatrixPtrRange(const ManagedMatrixPtrRange<T>& rhs) :
        ManagedMatrixPtrRange(static_cast<const MatrixPtrRange<T>&>(rhs)) {}
    ManagedMatrixPtrRange(const MatrixPtrRange<T>& rhs)
        : ManagedMatrixPtrRange(rhs.rows, rhs.cols) {
        auto itL = this->begin();
        auto itR = rhs.begin();
        auto endR = rhs.end();
        for (; itR != endR; ++itR, ++itL)
            *itL = *itR;
    }
    ManagedMatrixPtrRange(ManagedMatrixPtrRange&& rhs)
        : MatrixPtrRange<T>(std::exchange(rhs.first, nullptr),
                            std::exchange(rhs.last, nullptr),
                            rhs.rows, rhs.cols) {}

    ManagedMatrixPtrRange& operator=(const ManagedMatrixPtrRange<T>& rhs) {
        *this = static_cast<const MatrixPtrRange<T>&>(rhs);
    }

    ManagedMatrixPtrRange& operator=(const MatrixPtrRange<T>& rhs) {
        if (this->size() >= rhs.size()) {
            this->last = this->first + rhs.size();
            this->rows = rhs.rows;
            this->cols = rhs.cols;
        }
        else {
            this->~ManagedMatrixPtrRange();
            new (this) ManagedMatrixPtrRange(rhs.rows, rhs.cols);
        }
        auto itL = this->begin();
        auto itR = rhs.begin();
        auto endR = rhs.end();
        for (; itR != endR; ++itR, ++itL)
            *itL = *itR;
    }
    ManagedMatrixPtrRange& operator=(ManagedMatrixPtrRange&& rhs) {
        this->~ManagedMatrixPtrRange();
        this->first = std::exchange(rhs.first, nullptr);
        this->last = std::exchange(rhs.last, nullptr);
        return *this;
    }
    ~ManagedMatrixPtrRange() { delete[] this->first; }
};

enum class BaseVectorType {
    Resizeable, NonResizeable
};
template<typename T, BaseVectorType Type>
struct BaseVector : public ManagedPtrRange<T> {
    T* allocEnd;
    constexpr size_t capacity() const { return allocEnd - this->first; }
    BaseVector(size_t capacity, size_t sz)
        : ManagedPtrRange<T>(capacity) {
        assert(capacity > 0, "can't have zero capacity");
        allocEnd = this->last;
        this->last = this->first+sz;
    }
    explicit BaseVector(size_t capacity) : BaseVector(capacity, 0) {}
    BaseVector() : ManagedPtrRange<T>(nullptr) {}
    BaseVector(std::nullptr_t) : ManagedPtrRange<T>(nullptr), allocEnd(nullptr) {}
    BaseVector(std::initializer_list<T> l) : ManagedPtrRange<T>(l), allocEnd(this->last) {}

    void push_back(const T& val) {
        if constexpr (Type == BaseVectorType::Resizeable) {
            if (this->first == nullptr) {
                new (this) BaseVector<T, Type>(2);
            }
            else if (this->last >= allocEnd) {
                //std::cerr << "RESIZING" << "\n";
                size_t newCapacity = capacity() *2;
                T* newData = new T[newCapacity];
                size_t sz = this->size();
                for (size_t i = 0; i < sz; ++i) {
                    newData[i] = std::move(this->first[i]);
                }
                delete this->first;
                this->first = newData;
                this->last = newData + sz;
                this->allocEnd = newData + newCapacity;
            }
        } else {
            assert(this->last != allocEnd, "Overflow");
        }
        *(this->last++) = val;
    }
    constexpr bool empty() const {return this->size() == 0; }
    void insert(T* pos, const T& val) {
        assert(this->last != allocEnd, "Overflow");
        for (auto it = this->last-1; it != pos; --it)
            *it = *(it-1);
        ++this->last;
        *pos = val;
    }
    void remove(T* pos) {
        assert(this->last != this->first, "Empty vector");
        for (auto it = pos; it != this->last; ++it)
            *it = *(it+1);
        --this->last;
    }

    void pop_front() { remove(this->first); }
    T& front() { return *(this->first); }
    const T& front() const { return *(this->first); }
    T& back() { return *(this->last-1); }
    const T& back() const { return *(this->last-1); }
};

template<typename T>
using StaticVector = BaseVector<T, BaseVectorType::NonResizeable>;

template<typename T>
using Vector = BaseVector<T, BaseVectorType::Resizeable>;

template<typename T>
struct SimpleQueue {
    T* first;
    T* last;
    constexpr void push(const T& val) {
        *(last++) = val;
    }

    T pop() {
        assert(first < last, "Queue empty");
        return *(first++);
    }
    constexpr bool empty() const {
        return first == last;
    }
};
