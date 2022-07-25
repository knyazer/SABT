//
// Created by knyaz on 25/07/22.
//

#include "gtest/gtest.h"

#include <SABT.h>

#define N 100000

TEST(Shape3d, CubeVSCubeTest) {
    size_t incorrect = 0;
    for (size_t i = 0; i < N; i++) {
        size_t firstCubeSize = rand() % 100 + 1, secondCubeSize = rand() % 100 + 1;

        Vec3i firstCubePos = {rand() % 10000 - 5000, rand() % 10000 - 5000, rand() % 10000 - 5000};
        Vec3i secondCubePos = firstCubePos + Vec3i({rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50});

        Cube first(firstCubePos, firstCubeSize), second(secondCubePos, secondCubeSize);

        if (Cube::cubeIntersectsCube(first, second) != Shape3d::hasIntersection(&first, &second))
            incorrect++;
    }

    for (size_t i = 0; i < N; i++) {
        size_t firstCubeSize = rand() % 100 + 1, secondCubeSize = rand() % 100 + 1;

        Vec3i firstCubePos = {rand() % 200 - 100, rand() % 200 - 100, rand() % 200 - 100};
        Vec3i secondCubePos = {rand() % 200 - 100, rand() % 200 - 100, rand() % 200 - 100};

        Cube first(firstCubePos, firstCubeSize), second(secondCubePos, secondCubeSize);

        if (Cube::cubeIntersectsCube(first, second) != Shape3d::hasIntersection(&first, &second))
            incorrect++;
    }

    // Ignore boundary conditions mistakes - MPR returns arbitrary result on boundary
    ASSERT_TRUE(incorrect < (N / 1000));
}