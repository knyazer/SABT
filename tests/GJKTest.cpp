//
// Created by knyaz on 6/30/2022.
//

#include "gtest/gtest.h"

#include <AlignedRect.h>
#include <GJK.h>
#include <iostream>

#define N 100000

double r() {
    return ((double)(std::rand() * 2 - RAND_MAX) / (double)(std::rand() + 1));
}

TEST(GJK, AlignedRectVSAlignedRectTest) {
    double x1, x2, y1, y2;

    for (size_t i = 0; i < N; i++) {
        x1 = r();
        x2 = r();
        y1 = r();
        y2 = r();

        if (x1 > x2)
            std::swap(x1, x2);
        if (y1 > y2)
            std::swap(y1, y2);

        AlignedRect A({x1, y1}, {x2, y2});

        x1 = r();
        x2 = r();
        y1 = r();
        y2 = r();

        if (x1 > x2)
            std::swap(x1, x2);
        if (y1 > y2)
            std::swap(y1, y2);

        AlignedRect B({x1, y1}, {x2, y2});

        ASSERT_EQ(A.intersects(B), GJK::GJK(A, B, B.mid() - A.mid()));
    }
}

TEST(GJK, TriangleBoundingBoxTest) {
    for (size_t i = 0; i < N; i++) {
        Polygon A({{r(), r()}, {r(), r()}, {r(), r()}});
        Polygon B({{r(), r()}, {r(), r()}, {r(), r()}});

        // If no bounding box intersection - no shapes intersection
        if (!AlignedRect(A.vertices).intersects(AlignedRect(B.vertices))) {
            ASSERT_EQ(GJK::GJK(A, B), false);
        }
    }
}