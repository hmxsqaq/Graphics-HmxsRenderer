#ifndef VECTOR_H
#define VECTOR_H

#include <array>
#include <cassert>
#include <cmath>
#include <ostream>

template<typename T, size_t N>
struct Vector {
    Vector() { for (size_t i = 0; i < N; ++i) data_[i] = T(); }
    Vector(std::initializer_list<T> list) {
        size_t i = 0;
        for (const auto &element : list) {
            if (i >= N) break;
            data_[i++] = element;
        }
    }

    T& operator[](const size_t i)       { assert(i < N); return data_[i]; }
    T  operator[](const size_t i) const { assert(i < N); return data_[i]; }

    Vector operator+(const Vector &other) const {
        Vector ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data_[i] + other[i];
        return ret;
    }

    Vector operator-(const Vector &other) const {
        Vector ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data_[i] - other[i];
        return ret;
    }

    Vector operator*(const T &scalar) const {
        Vector ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data_[i] * scalar;
        return ret;
    }

    Vector operator/(const T &scalar) const {
        assert(scalar != 0);
        Vector ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data_[i] / scalar;
        return ret;
    }

    T operator*(const Vector &other) const {
        T ret = T();
        for (size_t i = 0; i < N; ++i) ret += data_[i] * other[i];
        return ret;
    }

    T dot(const Vector &other) const {
        return *this * other;
    }

    T magnitude() const { return std::sqrt(*this * *this); }

    Vector normalize() const { return *this / magnitude(); }

    template<size_t NEW_N>
    Vector<T, NEW_N> resize(T fill = T(1)) {
        if constexpr (N == NEW_N) return *this;
        return N > NEW_N ? project<NEW_N>() : embed<NEW_N>(fill);
    }

    template<size_t NEW_N>
    Vector<T, NEW_N> embed(T fill = T(1)) {
        assert(N <= NEW_N);
        Vector<T, NEW_N> ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data_[i];
        for (size_t i = N; i < NEW_N; ++i) ret[i] = fill;
        return ret;
    }

    template<size_t NEW_N>
    Vector<T, NEW_N> project() {
        assert(N >= NEW_N);
        Vector<T, NEW_N> ret;
        for (size_t i = 0; i < NEW_N; ++i) ret[i] = data_[i];
        return ret;
    }

    static T cross(const Vector<T, 2> &v1, const Vector<T, 2> &v2) { return v1[0] * v2[1] - v1[1] * v2[0]; }

    static Vector<T, 3> cross(const Vector<T, 3> &v1, const Vector<T, 3> &v2) {
        return Vector<T, 3>({
            v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]
        });
    }
private:
    std::array<T, N> data_;
};

template<typename T, size_t N>
std::ostream& operator<<(std::ostream &out, const Vector<T, N> &vec) {
    out << "[";
    for (size_t i = 0; i < N; ++i) {
        out << vec[i];
        if (i < N - 1) out << ", ";
    }
    out << "]";
    return out;
}

using Vector2 = Vector<double, 2>;
using Vector3 = Vector<double, 3>;
using Vector4 = Vector<double, 4>;
using Vector2I = Vector<int, 2>;
using Vector3I = Vector<int, 3>;
using Vector4I = Vector<int, 4>;

#endif //VECTOR_H
