//
// Created by knyaz on 7/1/2022.
//
// Matrix class, any matrix size supported, though only for doubles.
// Interesting fact - operations with wrong matrix shapes produce runtime errors,
// not a compilation time errors, because it is simpler to test them and/or ignore them.
//

#ifndef SABT_MAT_H
#define SABT_MAT_H

#include <vector>
#include <stdexcept>
#include <iostream>

template<size_t R, size_t C>
class Mat {
private:
    double data[R][C]{};

    constexpr const static double eps = 1e-4;

public:

    Mat() = default;

    Mat(std::vector<std::vector<double>> mat) {
        if (mat.empty())
            return;

        if (mat.size() != R || mat[0].size() != C)
            throw std::runtime_error("Given vector to a matrix constructor is of wrong shape");

        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                data[i][j] = mat[i][j];
            }
        }
    }

    inline void fill(double value) {
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                data[i][j] = value;
            }
        }
    }

    [[nodiscard]] inline double &at(size_t i, size_t j) {
        if (i < 0 || i >= R || j < 0 || j >= C)
            throw std::runtime_error("Out of bounds of matrix.");

        return data[i][j];
    }

    [[nodiscard]] inline double qat(size_t i, size_t j) const {
        return data[i][j];
    }

    // The correct matrix operation - sum with matrix of same dimensions
    Mat<R, C> operator+(Mat<R, C> other) {
        Mat<R, C> res;
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                res.at(i, j) = at(i, j) + other.at(i, j);
            }
        }

        return res;
    }

    // The correct matrix operation - sum with scalar (bad but alright)
    template<typename T>
    Mat<R, C> operator+(T other) {
        Mat<R, C> res;
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                res.at(i, j) = at(i, j) + other;
            }
        }

        return res;
    }

    // The overload of all other matrix sizes - instantly throw an error
    template<size_t oR, size_t oC>
    Mat operator+(Mat<oR, oC> other) {
        throw std::runtime_error("Matrices to be add should be exactly the same size.");
    }

    // The correct matrix operation - subtraction with matrix of same dimensions
    Mat<R, C> operator-(Mat<R, C> other) {
        Mat<R, C> res;
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                res.at(i, j) = at(i, j) - other.at(i, j);
            }
        }

        return res;
    }

    // The correct matrix operation - subtraction with scalar (bad but alright)
    template<typename T>
    Mat<R, C> operator-(T other) {
        Mat<R, C> res;
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                res.at(i, j) = at(i, j) - other;
            }
        }

        return res;
    }

    // The overload of all other matrix sizes - instantly throw an error
    template<size_t oW, size_t oH>
    Mat operator-(Mat<oW, oH> other) {
        throw std::runtime_error("Matrices to be subtracted should be exactly the same size.");
    }

    // The correct operation - number of columns of A equals to number of rows of B
    // RxC * CxN = RxN
    template<size_t N>
    Mat<R, N> operator*(Mat<C, N> other) {
        Mat<R, N> result;
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < N; j++) {

                result.at(i, j) = 0;
                for (size_t k = 0; k < C; k++) {
                    result.at(i, j) += at(i, k) * other.at(k, j);
                }

            }
        }

        return result;
    }

    // Overload of a wrong shapes operation
    template<size_t M, size_t N>
    Mat operator*(Mat<M, N> other) {
        throw std::runtime_error(
                "Number of columns of first matrix should be equal to number of rows of second matrix, "
                "matrix multiplication failed.");
    }

    // Get access to data in a row
    double &operator[](size_t i) {
        if (i >= R * C)
            throw std::runtime_error("Attempt to access value out of matrix bounds using [].");

        return data[i >> 2][i & 0b11];
    }

    // Generate & return the inverse of given matrix
    bool inverse(Mat<R, C> &invOut) {
        if (R != C)
            throw std::runtime_error("Cannot take inverse of the matrix with rows != columns");

        if (R != 4 || C != 4)
            throw std::runtime_error("Currently the inverse does not work with other then 4x4 matrices");

        Mat<4, 4> inv;
        double det;
        int i;

        auto m = [this](size_t i) { return this->operator[](i); };

        inv[0] = m(5) * m(10) * m(15) -
                 m(5) * m(11) * m(14) -
                 m(9) * m(6) * m(15) +
                 m(9) * m(7) * m(14) +
                 m(13) * m(6) * m(11) -
                 m(13) * m(7) * m(10);

        inv[4] = -m(4) * m(10) * m(15) +
                 m(4) * m(11) * m(14) +
                 m(8) * m(6) * m(15) -
                 m(8) * m(7) * m(14) -
                 m(12) * m(6) * m(11) +
                 m(12) * m(7) * m(10);

        inv[8] = m(4) * m(9) * m(15) -
                 m(4) * m(11) * m(13) -
                 m(8) * m(5) * m(15) +
                 m(8) * m(7) * m(13) +
                 m(12) * m(5) * m(11) -
                 m(12) * m(7) * m(9);

        inv[12] = -m(4) * m(9) * m(14) +
                  m(4) * m(10) * m(13) +
                  m(8) * m(5) * m(14) -
                  m(8) * m(6) * m(13) -
                  m(12) * m(5) * m(10) +
                  m(12) * m(6) * m(9);

        inv[1] = -m(1) * m(10) * m(15) +
                 m(1) * m(11) * m(14) +
                 m(9) * m(2) * m(15) -
                 m(9) * m(3) * m(14) -
                 m(13) * m(2) * m(11) +
                 m(13) * m(3) * m(10);

        inv[5] = m(0) * m(10) * m(15) -
                 m(0) * m(11) * m(14) -
                 m(8) * m(2) * m(15) +
                 m(8) * m(3) * m(14) +
                 m(12) * m(2) * m(11) -
                 m(12) * m(3) * m(10);

        inv[9] = -m(0) * m(9) * m(15) +
                 m(0) * m(11) * m(13) +
                 m(8) * m(1) * m(15) -
                 m(8) * m(3) * m(13) -
                 m(12) * m(1) * m(11) +
                 m(12) * m(3) * m(9);

        inv[13] = m(0) * m(9) * m(14) -
                  m(0) * m(10) * m(13) -
                  m(8) * m(1) * m(14) +
                  m(8) * m(2) * m(13) +
                  m(12) * m(1) * m(10) -
                  m(12) * m(2) * m(9);

        inv[2] = m(1) * m(6) * m(15) -
                 m(1) * m(7) * m(14) -
                 m(5) * m(2) * m(15) +
                 m(5) * m(3) * m(14) +
                 m(13) * m(2) * m(7) -
                 m(13) * m(3) * m(6);

        inv[6] = -m(0) * m(6) * m(15) +
                 m(0) * m(7) * m(14) +
                 m(4) * m(2) * m(15) -
                 m(4) * m(3) * m(14) -
                 m(12) * m(2) * m(7) +
                 m(12) * m(3) * m(6);

        inv[10] = m(0) * m(5) * m(15) -
                  m(0) * m(7) * m(13) -
                  m(4) * m(1) * m(15) +
                  m(4) * m(3) * m(13) +
                  m(12) * m(1) * m(7) -
                  m(12) * m(3) * m(5);

        inv[14] = -m(0) * m(5) * m(14) +
                  m(0) * m(6) * m(13) +
                  m(4) * m(1) * m(14) -
                  m(4) * m(2) * m(13) -
                  m(12) * m(1) * m(6) +
                  m(12) * m(2) * m(5);

        inv[3] = -m(1) * m(6) * m(11) +
                 m(1) * m(7) * m(10) +
                 m(5) * m(2) * m(11) -
                 m(5) * m(3) * m(10) -
                 m(9) * m(2) * m(7) +
                 m(9) * m(3) * m(6);

        inv[7] = m(0) * m(6) * m(11) -
                 m(0) * m(7) * m(10) -
                 m(4) * m(2) * m(11) +
                 m(4) * m(3) * m(10) +
                 m(8) * m(2) * m(7) -
                 m(8) * m(3) * m(6);

        inv[11] = -m(0) * m(5) * m(11) +
                  m(0) * m(7) * m(9) +
                  m(4) * m(1) * m(11) -
                  m(4) * m(3) * m(9) -
                  m(8) * m(1) * m(7) +
                  m(8) * m(3) * m(5);

        inv[15] = m(0) * m(5) * m(10) -
                  m(0) * m(6) * m(9) -
                  m(4) * m(1) * m(10) +
                  m(4) * m(2) * m(9) +
                  m(8) * m(1) * m(6) -
                  m(8) * m(2) * m(5);

        det = m(0) * inv[0] + m(1) * inv[4] + m(2) * inv[8] + m(3) * inv[12];

        if (det == 0)
            return false;

        det = 1.0 / det;

        for (i = 0; i < 16; i++)
            invOut[i] = inv[i] * det;

        if (*this * invOut != I()) {
            std::cerr << *this << " * " << invOut << " = " << (*this * invOut) << std::endl;
            throw std::runtime_error("Failed to build correct inverse.");
        }

        return true;
    }

    // Equality of matrices is not the great way to compare them, but is quickest to use
    bool operator==(Mat<R, C> other) {
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                if (std::abs(at(i, j) - other.at(i, j)) > eps) {
                    return false;
                }
            }
        }

        return true;
    }

    template<typename T>
    bool operator==(T other) {
        return false;
    }

    // Identity matrix builder
    Mat<R, C> I() {
        if (R != C) {
            throw std::runtime_error("Cannot build not squared identity matrix.");
        }

        Mat<R, C> res;
        for (size_t i = 0; i < R; i++) {
            res.at(i, i) = 1;
        }

        return res;
    }

    friend std::ostream &operator<<(std::ostream &os, const Mat<R, C> &val) {
        os << "Mat<" << R << ", " << C << ">" << std::endl;
        for (size_t i = 0; i < R; i++) {
            os << "[";
            for (size_t j = 0; j < C; j++) {
                os << val.data[i][j];
                if (j != C - 1)
                    os << "\t";
            }
            os << "]" << std::endl;
        }

        return os;
    }

    ~Mat() = default;
};

#endif //SABT_MAT_H
