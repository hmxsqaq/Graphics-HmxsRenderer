#ifndef MATRIX_H
#define MATRIX_H

#include <array>
#include "vector.h"

template<typename T, size_t N> struct Determinant;

template<typename T, size_t ROW, size_t COL>
struct Matrix {
    Matrix() { for (size_t i = 0; i < ROW; ++i) data_[i] = Vector<T, COL>(); }
    Matrix(std::initializer_list<Vector<T, COL>> list) {
        size_t i = 0;
        for (const auto &element : list) {
            if (i >= ROW) break;
            data_[i++] = element;
        }
    }

    Vector<T, COL>&       operator[](const size_t i)       { assert(i < ROW); return data_[i]; }
    const Vector<T, COL>& operator[](const size_t i) const { assert(i < ROW); return data_[i]; }

    Matrix operator+(const Matrix &other) const {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data_[i] + other[i];
        return ret;
    }

    Matrix operator-(const Matrix &other) const {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data_[i] - other[i];
        return ret;
    }

    Matrix operator*(const T &scalar) const {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data_[i] * scalar;
        return ret;
    }

    Matrix operator/(const T &scalar) const {
        assert(scalar != 0);
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data_[i] / scalar;
        return ret;
    }

    Vector<T, ROW> operator*(const Vector<T, COL> &vec) const {
        Vector<T, ROW> ret;
        for (size_t i = 0; i < ROW; ++i) ret[i] = data_[i] * vec;
        return ret;
    }

    // [ROW, COL] * [COL, NEW_COL] = [ROW, NEW_COL]
    template<size_t NEW_COL>
    Matrix<T, ROW, NEW_COL> operator*(const Matrix<T, COL, NEW_COL> &other) const {
        Matrix<T, ROW, NEW_COL> ret;
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < NEW_COL; ++j)
                ret[i][j] = data_[i] * other.getCol(j);
        return ret;
    }

    Vector<T, ROW> getCol(const size_t i) const {
        assert(i < COL);
        Vector<T, ROW> ret;
        for (size_t j = 0; j < ROW; ++j) ret[j] = data_[j][i];
        return ret;
    }

    void setCol(const size_t i, const Vector<T, ROW> &col) {
        assert(i < COL);
        for (size_t j = 0; j < ROW; ++j) data_[j][i] = col[j];
    }

    Matrix<T, ROW - 1, COL - 1> getMinor(const size_t row, const size_t col) const {
        Matrix<T, ROW - 1, COL - 1> ret;
        for (size_t i = 0; i < ROW - 1; ++i)
            for (size_t j = 0; j < COL - 1; ++j)
                ret[i][j] = data_[i < row ? i : i + 1][j < col ? j : j + 1];
        return ret;
    }

    Matrix<T, COL, ROW> getTranspose() const {
        Matrix<T, COL, ROW> ret;
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                ret[j][i] = data_[i][j];
        return ret;
    }

    T getDeterminant() const { return Determinant<T, ROW>::calculate(*this); }

    T getCofactor(const size_t row, const size_t col) const {
        return getMinor(row, col).getDeterminant() * ((row + col) % 2 ? -1 : 1);
    }

    Matrix getAdjointTranspose() const {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i)
            for (size_t j = 0; j < COL; ++j)
                ret[i][j] = getCofactor(i, j);
        return ret;
    }

    Matrix getAjoint() const { return getAdjointTranspose().getTranspose(); }

    Matrix getInverseTranspose() const {
        Matrix ret = getAdjointTranspose();
        T determinant = ret[0] * (*this)[0];
        assert(determinant != 0);
        return ret / determinant;
    }

    Matrix getInverse() const { return getInverseTranspose().getTranspose(); }

    static Matrix identity() {
        Matrix ret;
        for (size_t i = 0; i < ROW; ++i) ret[i][i] = 1;
        return ret;
    }

private:
    std::array<Vector<T, COL>, ROW> data_;
};

template<typename T, size_t ROW, size_t COL>
std::ostream& operator<<(std::ostream &out, const Matrix<T, ROW, COL> matrix) {
    for (size_t i = 0; i < ROW; ++i) out << matrix[i] << "\n";
    return out;
}

template<typename T, size_t N>
struct Determinant {
    static T calculate(const Matrix<T, N, N> &matrix) {
        T ret = T();
        for (size_t i = 0; i < N; ++i) ret += matrix[0][i] * matrix.getCofactor(0, i);
        return ret;
    }
};

template<typename T>
struct Determinant<T, 1> {
    static T calculate(const Matrix<T, 1, 1> &matrix) { return matrix[0][0]; }
};

template<typename T>
struct Determinant<T, 2> {
    static T calculate(const Matrix<T, 2, 2> &matrix) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
};

using Matrix2x2 = Matrix<double, 2, 2>;
using Matrix3x3 = Matrix<double, 3, 3>;
using Matrix4x4 = Matrix<double, 4, 4>;

#endif //MATRIX_H