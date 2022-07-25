//
// Created by knyaz on 25/07/22.
//

#include "gtest/gtest.h"

#include <SABT.h>

TEST(Shape3d, CubeVSCubeTest) {
/*    for (size_t i = 0; i < 1000; i++) {
        size_t firstCubeSize = rand() % 100, secondCubeSize = rand() % 100;

        Vec3i firstCubePos = {rand() - RAND_MAX / 2, rand() - RAND_MAX / 2, rand() - RAND_MAX / 2};
        Vec3i secondCubePos = firstCubePos + Vec3i({rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50});

        Cube first(firstCubePos, firstCubeSize), second(secondCubePos, secondCubeSize);
        EXPECT_EQ(Cube::cubeIntersectsCube(first, second), Shape3d::hasIntersection(&first, &second));
    }*/
}