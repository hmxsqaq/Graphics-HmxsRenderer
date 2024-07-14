#ifndef VECTOR_H
#define VECTOR_H

#include <array>
#include <cassert>

template<typename T, size_t N>
struct Vec {
    Vec() { for (int i = 0; i < N; ++i) data[i] = T(); }
    Vec(std::initializer_list<T> list) {
        int i = 0;
        for (const auto &element : list) {
            if (i >= N) break;
            data[i++] = element;
        }
    }

    T& operator[](const int i)       { assert(i >= 0 && i < N); return data[i]; }
    T  operator[](const int i) const { assert(i >= 0 && i < N); return data[i]; }

    Vec operator+(const Vec &other) const {
        Vec ret;
        for (int i = 0; i < N; ++i) ret[i] = data[i] + other[i];
        return ret;
    }

    Vec operator-(const Vec &other) const {
        Vec ret;
        for (int i = 0; i < N; ++i) ret[i] = data[i] - other[i];
        return ret;
    }

    Vec operator*(const T &scalar) const {
        Vec ret;
        for (int i = 0; i < N; ++i) ret[i] = data[i] * scalar;
        return ret;
    }
private:
    std::array<T, N> data;
};


#endif //VECTOR_H
