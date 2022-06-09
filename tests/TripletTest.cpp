//
// Created by knyaz on 6/8/2022.
//

#include "gtest/gtest.h"

#include <Triplet.h>
#include <stdexcept>

TEST(Triplet, GeneralTest) {
    Triplet tri;

    ASSERT_ANY_THROW(tri.set(12));

    tri.set(1);

    ASSERT_ANY_THROW(tri.set(-1));

    ASSERT_EQ(tri.index(), 1);

    for (int i = 0; i < 8; i++) {
        tri.set(i);
        ASSERT_EQ(tri.index(), i);
    }

    for (int x = -1; x <= 1; x += 2) {
        for (int y = -1; y <= 1; y += 2) {
            for (int z = -1; z <= 1; z += 2) {
                tri.set(x, y, z);

                ASSERT_EQ(tri.x(), x);
                ASSERT_EQ(tri.y(), y);
                ASSERT_EQ(tri.z(), z);
            }
        }
    }
}
