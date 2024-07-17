#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include "vector.h"

template<typename T, size_t N> struct Determinant;

template<typename T, size_t ROW, size_t COL>
struct Matrix {
    Matrix() { for (size_t i = 0; i < ROW; ++i) data[i] = Vector<T, COL>(); }
    Matrix(std::initializer_list<Vector<T, COL>> list) {
        size_t i = 0;
        for (const auto &element : list) {
            if (i >= ROW) break;
            data[i++] = element;
        }
    }

    Vector<T, COL>&       operator[](const size_t i)       { assert(i < ROW); return data[i]; }
    const Vector<T, COL>& operator[](const size_t i) const { assert(i < ROW); return data[i]; }

    Matrix operator+(const Matrix &other) const {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data[i] + other[i];
        return ret;
    }

    Matrix operator-(const Matrix &other) const {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data[i] - other[i];
        return ret;
    }

    Matrix operator*(const T &scalar) const {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data[i] * scalar;
        return ret;
    }

    Matrix operator/(const T &scalar) const {
        assert(scalar != 0);
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data[i] / scalar;
        return ret;
    }

    Vector<T, ROW> operator*(const Vector<T, COL> &vec) const {
        Vector<T, ROW> ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data[i] * vec;
        return ret;
    }

    // [ROW, COL] * [COL, NEW_COL] = [ROW, NEW_COL]
    template<size_t NEW_COL>
    Matrix<T, ROW, NEW_COL> operator*(const Matrix<T, COL, NEW_COL> &other) const {
        Matrix<T, ROW, NEW_COL> ret;
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < NEW_COL; ++j)
                ret[i][j] = data[i] * other.Col(j);
        return ret;
    }

    Vector<T, ROW> Col(const size_t i) const {
        assert(i < COL);
        Vector<T, ROW> ret;
        for (size_t j = 0; j < ROW; ++j) ret[j] = data[j][i];
        return ret;
    }

    void SetCol(const size_t i, const Vector<T, ROW> &col) {
        assert(i < COL);
        for (size_t j = 0; j < ROW; ++j) data[j][i] = col[j];
    }

    Matrix<T, ROW - 1, COL - 1> Minor(const size_t row, const size_t col) const {
        Matrix<T, ROW - 1, COL - 1> ret;
        for (size_t i = 0; i < ROW - 1; ++i)
            for (size_t j = 0; j < COL - 1; ++j)
                ret[i][j] = data[i < row ? i : i + 1][j < col ? j : j + 1];
        return ret;
    }

    Matrix<T, COL, ROW> Transpose() const {
        Matrix<T, COL, ROW> ret;
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                ret[j][i] = data[i][j];
        return ret;
    }

    T CalculateDeterminant() const { return Determinant<T, ROW>::Calculate(*this); }

    T Cofactor(const size_t row, const size_t col) const {
        return Minor(row, col).CalculateDeterminant() * ((row + col) % 2 ? -1 : 1);
    }

    Matrix AdjointTranspose() const {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                ret[i][j] = Cofactor(i, j);
        return ret;
    }

    Matrix Ajoint() const { return AdjointTranspose().Transpose(); }

    Matrix InverseTranspose() const {
        Matrix ret = AdjointTranspose();
        T determinant = ret[0] * (*this)[0];
        assert(determinant != 0);
        return ret / determinant;
    }

    Matrix Inverse() const { return InverseTranspose().Transpose(); }

    static Matrix Identity() {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i][i] = 1;
        return ret;
    }

    std::array<Vector<T, COL>, ROW> data;
};

template<typename T, size_t ROW, size_t COL>
std::ostream& operator<<(std::ostream &out, const Matrix<T, ROW, COL> matrix) {
    for (size_t i = 0; i < ROW; ++i) out << matrix[i] << "\n";
    return out;
}

template<typename T, size_t N>
struct Determinant {
    static T Calculate(const Matrix<T, N, N> &matrix) {
        T ret = T();
        for (size_t i = 0; i < N; ++i) ret += matrix[0][i] * matrix.Cofactor(0, i);
        return ret;
    }
};

template<typename T>
struct Determinant<T, 1> {
    static T Calculate(const Matrix<T, 1, 1> &matrix) { return matrix[0][0]; }
};

template<typename T>
struct Determinant<T, 2> {
    static T Calculate(const Matrix<T, 2, 2> &matrix) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
};

using Matrix2x2 = Matrix<double, 2, 2>;
using Matrix3x3 = Matrix<double, 3, 3>;
using Matrix4x4 = Matrix<double, 4, 4>;

#endif //MATRIX_H