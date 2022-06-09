//
// Created by knyaz on 6/8/2022.
//

#include "gtest/gtest.h"

#include <Octree.h>
#include <OctreeRoot.h>

TEST(Octree, SimpleFillingTest) {
    Octree octree;

    ASSERT_EQ(octree.isEmpty(), true);
    ASSERT_EQ(octree.isFull(), false);

    octree.fill();

    ASSERT_EQ(octree.isEmpty(), false);
    ASSERT_EQ(octree.isFull(), true);

    octree.clear();

    ASSERT_EQ(octree.isEmpty(), true);
    ASSERT_EQ(octree.isFull(), false);
}

TEST(Octree, SingleFillTest) {
    OctreeRoot octree;

    octree.grow();
    octree.grow();

    octree.fill(Vec3i(0, 0, 0), 4);

    ASSERT_EQ(octree.isEmpty(), false);
    ASSERT_EQ(octree.isFull(), false);

    ASSERT_EQ(octree.getChild({-1, -1, -1}).isEmpty(), true);
    ASSERT_EQ(octree.getChild({-1, -1, -1}).isFull(), false);

    ASSERT_EQ(octree.getChild({1, 1, 1}).isEmpty(), false);
    ASSERT_EQ(octree.getChild({1, 1, 1}).isFull(), false);

    ASSERT_EQ(octree.getChild({1, 1, 1}).getChild({-1, -1, -1}).isEmpty(), false);
    ASSERT_EQ(octree.getChild({1, 1, 1}).getChild({-1, -1, -1}).isFull(), true);
}

TEST(Octree, SingleGrowthTest) {
    OctreeRoot octree;

    octree.grow();
    octree.grow();

    octree.fill(Vec3i(0, 0, 0), 4);

    octree.grow();
    octree.grow();

    Octree child = octree;
    for (int i = 1; i <= 16; i *= 2) {
        if (i != 16) {
            ASSERT_EQ(child.isSemi(), true);
        } else {
            ASSERT_EQ(child.isFull(), true);
        }

        if (i == 1) {
            child = child.getChild({1, 1, 1});
        } else if (i != 16) {
            child = child.getChild({-1, -1, -1});
        }
    }
}
