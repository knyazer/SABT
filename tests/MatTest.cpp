//
// Created by knyaz on 7/1/2022.
//

#include "gtest/gtest.h"

#include <Mat.h>

TEST(Mat, ReadWriteTest) {
    Mat<10, 10> mat;

    for (int j = 0; j < 10; j++)
        for (int i = 0; i < 10; i++)
            mat.at(i, j) = (i + 1) * (j + 1);

    for (int j = 0; j < 10; j++)
        for (int i = 0; i < 10; i++)
            ASSERT_EQ(mat.at(i, j), (i + 1) * (j + 1));
}

TEST(Mat, MatOperationsTest) {
    Mat<3, 3> a, b, ans;

    auto af = [](auto i, auto j) { return (i + 2.7) * (j - 1.6); };
    auto bf = [](auto i, auto j) { return (i * 2.2) * (j + 3.3); };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            a.at(i, j) = af(i, j);
            b.at(i, j) = bf(i, j);
        }
    }

    ans = a + b;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_EQ(ans.at(i, j), af(i, j) + bf(i, j));
        }
    }

    ans = a - b;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            ASSERT_EQ(ans.at(i, j), af(i, j) - bf(i, j));
        }
    }

    ans = a * b;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            double res = 0;
            for (int k = 0; k < 3; k++) {
                res += af(i, k) * bf(k, j);
            }

            ASSERT_EQ(ans.at(i, j), res);
        }
    }
}

TEST(Mat, MatThrowTest) {
    Mat<3, 2> a;
    Mat<3, 5> b;

    EXPECT_THROW({
                     try {
                         auto res = a + b;
                     }
                     catch (const std::exception &e) {
                         EXPECT_STREQ("Matrices to be add should be exactly the same size.", e.what());
                         throw;
                     }
                 }, std::exception);

    EXPECT_THROW({
                     try {
                         auto res = a - b;
                     }
                     catch (const std::exception &e) {
                         EXPECT_STREQ("Matrices to be subtracted should be exactly the same size.", e.what());
                         throw;
                     }
                 }, std::exception);

    EXPECT_THROW({
                     try {
                         auto res = a * b;
                     }
                     catch (const std::exception &e) {
                         EXPECT_STREQ("Number of columns of first matrix should be equal to number of rows of second matrix, "
                                      "matrix multiplication failed.", e.what());
                         throw;
                     }
                 }, std::exception);
}

TEST(Mat, MatDotFixedTest) {
    Mat<3,2> a({{0, 4}, {3, 7}, {8, 5}});
    Mat<2,4> b({{-2, 1, 5, 5}, {2, 4, 7, 6}});

    Mat<3,4> expected({{8, 16, 28, 24}, {8, 31, 64, 57}, {-6, 28, 75, 70}});

    ASSERT_EQ((expected == (a * b)), true);
}