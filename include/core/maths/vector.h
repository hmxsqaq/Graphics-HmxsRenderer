#ifndef VECTOR_H
#define VECTOR_H

#include <array>
#include <cassert>
#include <cmath>
#include <ostream>
#include <sstream>

/**
 * @brief Vector struct containing various vector operations.
 * @tparam T type of the vector.
 * @tparam N dimension of the vector.
 */
template<typename T, size_t N>
struct Vector {
    std::array<T, N> data;

    Vector() { for (size_t i = 0; i < N; ++i) data[i] = T(); }
    Vector(std::initializer_list<T> list) {
        size_t i = 0;
        for (const auto &element : list) {
            if (i >= N) break;
            data[i++] = element;
        }
    }

    T& operator[](const size_t i)       { assert(i < N); return data[i]; }
    T  operator[](const size_t i) const { assert(i < N); return data[i]; }

    Vector operator+(const Vector &other) const {
        Vector ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data[i] + other[i];
        return ret;
    }

    Vector operator-(const Vector &other) const {
        Vector ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data[i] - other[i];
        return ret;
    }

    Vector operator*(const T &scalar) const {
        Vector ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data[i] * scalar;
        return ret;
    }

    Vector operator/(const T &scalar) const {
        assert(scalar != 0);
        Vector ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data[i] / scalar;
        return ret;
    }

    T operator*(const Vector &other) const {
        T ret = T();
        for (size_t i = 0; i < N; ++i) ret += data[i] * other[i];
        return ret;
    }

    T Dot(const Vector &other) const {
        return *this * other;
    }

    T Magnitude() const { return std::sqrt(*this * *this); }

    Vector Normalize() const { return *this / Magnitude(); }

    template<size_t NEW_N>
    Vector<T, NEW_N> Resize(T fill = T(1)) const {
        if constexpr (N == NEW_N) return *this;
        return N > NEW_N ? Project<NEW_N>() : Embed<NEW_N>(fill);
    }

    template<size_t NEW_N>
    Vector<T, NEW_N> Embed(T fill = T(1)) const {
        assert(N <= NEW_N);
        Vector<T, NEW_N> ret;
        for (size_t i = 0; i < N; ++i) ret[i] = data[i];
        for (size_t i = N; i < NEW_N; ++i) ret[i] = fill;
        return ret;
    }

    template<size_t NEW_N>
    Vector<T, NEW_N> Project() const {
        assert(N >= NEW_N);
        Vector<T, NEW_N> ret;
        for (size_t i = 0; i < NEW_N; ++i) ret[i] = data[i];
        return ret;
    }

    static T Cross(const Vector<T, 2> &v1, const Vector<T, 2> &v2) { return v1[0] * v2[1] - v1[1] * v2[0]; }

    static Vector<T, 3> Cross(const Vector<T, 3> &v1, const Vector<T, 3> &v2) {
        return Vector<T, 3>({
            v1[1] * v2[2] - v1[2] * v2[1],
            v1[2] * v2[0] - v1[0] * v2[2],
            v1[0] * v2[1] - v1[1] * v2[0]
        });
    }

    static Vector Interpolate(const Vector &v1, const Vector &v2, const Vector &v3, const Vector<T, 3> &bc, float weight = 1.0f) {
        return (v1 * bc[0] + v2 * bc[1] + v3 * bc[2]) / weight;
    }

    [[nodiscard]] std::string ToString() const {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < N; ++i) {
            oss << data[i];
            if (i < N - 1) oss << ", ";
        }
        oss << "]";
        return oss.str();
    }
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

// pre-defined vector types
using Vector2f = Vector<float, 2>;
using Vector3f = Vector<float, 3>;
using Vector4f = Vector<float, 4>;
using Vector2i = Vector<int, 2>;
using Vector3i = Vector<int, 3>;
using Vector4i = Vector<int, 4>;
using Vector2s = Vector<size_t, 2>;
using Vector3s = Vector<size_t, 3>;
using Vector4s = Vector<size_t, 4>;

#endif //VECTOR_H
