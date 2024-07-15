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

template<typename T>
T cross(const Vec<T, 2> &v1, const Vec<T, 2> &v2) { return v1[0] * v2[1] - v1[1] * v2[0]; }

template<typename T>
Vec<T, 3> cross(const Vec<T, 3> &v1, const Vec<T, 3> &v2) {
    return Vec<T, 3>({
        v1[1] * v2[2] - v1[2] * v2[1],
        v1[2] * v2[0] - v1[0] * v2[2],
        v1[0] * v2[1] - v1[1] * v2[0]
    });
}

template<typename T, size_t N>
std::ostream& operator<<(std::ostream &out, const Vec<T, N> &vec) {
    out << "[";
    for (size_t i = 0; i < N; ++i) {
        out << vec[i];
        if (i < N - 1) out << ", ";
    }
    out << "]";
    return out;
}

using Vec2 = Vec<double, 2>;
using Vec3 = Vec<double, 3>;
using Vec4 = Vec<double, 4>;
using Vec2i = Vec<int, 2>;
using Vec3i = Vec<int, 3>;
using Vec4i = Vec<int, 4>;

#endif //VECTOR_H
