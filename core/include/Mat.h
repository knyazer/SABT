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

template<size_t R, size_t C>
class Mat {
private:
    double data[R][C]{};

    constexpr const static double eps = 1e-9;

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

    inline double &at(size_t i, size_t j) {
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

    // Equality of matrices is not the great way to compare them, but is quickest to use
    bool operator==(Mat<R, C> other) {
        for (size_t i = 0; i < R; i++) {
            for (size_t j = 0; j < C; j++) {
                if (abs(at(i, j) - other.at(i, j)) > eps) {
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

    ~Mat() = default;
};

#endif //SABT_MAT_H
