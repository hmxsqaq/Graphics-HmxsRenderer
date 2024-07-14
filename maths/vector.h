#ifndef VECTOR_H
#define VECTOR_H

#include <array>
#include <cassert>
#include <cmath>
#include <ostream>

template<typename T, size_t N>
struct Vec {
    Vec() { for (size_t i = 0; i < N; ++i) data_[i] = T(); }
    Vec(std::initializer_list<T> list) {
        size_t i = 0;
        for (const auto &element : list) {
            if (i >= N) break;
            data_[i++] = element;
        }
    }

    T& operator[](const size_t i)       { assert(i < N); return data_[i]; }
    T  operator[](const size_t i) const { assert(i < N); return data_[i]; }

    Vec operator+(const Vec &other) const {
        Vec ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data_[i] + other[i];
        return ret;
    }

    Vec operator-(const Vec &other) const {
        Vec ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data_[i] - other[i];
        return ret;
    }

    Vec operator*(const T &scalar) const {
        Vec ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data_[i] * scalar;
        return ret;
    }

    Vec operator/(const T &scalar) const {
        assert(scalar != 0);
        Vec ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data_[i] / scalar;
        return ret;
    }

    T dot(const Vec &other) const {
        T ret = T();
        for (size_t i = 0; i < N; ++i) ret += data_[i] * other[i];
        return ret;
    }

    T magnitude() const { return std::sqrt(dot(*this)); }
    Vec normalize() const { return *this / magnitude(); }

private:
    std::array<T, N> data_;
};

template<typename T, size_t N>
inline std::ostream& operator<<(std::ostream &out, const Vec<T, N> &vec) {
    out << "(";
    for (size_t i = 0; i < N; ++i) {
        out << vec[i];
        if (i < N - 1) out << ", ";
    }
    out << ")";
    return out;
}

#endif //VECTOR_H
