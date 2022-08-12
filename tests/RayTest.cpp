//
// Created by knyaz on 12/08/22.
//

#include "gtest/gtest.h"

#include <SABT.h>

#define N 50000

TEST(Ray, MPRvsQuickRayIntersectionTest) {
    long long incorrect = 0;

    for (size_t i = 0; i < N; i++) {
        Cube cube({rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5}, rand() % 5);

        Vec3f origin(rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5);
        Vec3f direction(origin.x - cube.pos.x, origin.y - cube.pos.y, origin.z - cube.pos.z);
        direction = direction + Vec3f(rand() % 5 - 2, rand() % 5 - 2, rand() % 5 - 2);

        Ray ray(origin, direction);

        EXPECT_EQ(Shape3d::hasIntersection(&ray, &cube), ray.hasIntersection(cube));
    }
}