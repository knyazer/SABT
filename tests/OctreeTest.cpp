//
// Created by knyaz on 6/8/2022.
//

#include "gtest/gtest.h"

#include <stack>
#include <vector>
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

    octree.fill(Vec3i(0, 0, 0), 0);

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

    octree.fill(Vec3i(0, 0, 0), 0);

    octree.grow();
    octree.grow();

    Octree* child = &octree;
    for (int i = 1; i <= 16; i *= 2) {
        if (i != 16) {
            ASSERT_EQ(child->isSemi(), true);
        } else {
            ASSERT_EQ(child->isFull(), true);
        }

        if (i == 1) {
            child = &child->getChild({1, 1, 1});
        } else if (i != 16) {
            child = &child->getChild({-1, -1, -1});
        }
    }
}

TEST(Octree, ParentalTest) {
    OctreeRoot octree;

    for (size_t i = 0; i < 8; i++)
        octree.grow();

    for (size_t i = 0; i < 100; i++) {
        int x = rand() % 7 - 4, y = rand() % 9 - 5, z = rand() % 11 - 3;
        int level = rand() % 3;

        for (size_t j = 0; j < level; j++) {
            x = (x / 2) * 2;
            y = (y / 2) * 2;
            z = (z / 2) * 2;
        }

        Octree* bottom = octree.fill({x, y, z}, level);
        Octree* top = &octree;

        // Test that ancestors with top->bottom are the same as with bottom->top
        Octree* ptr = bottom;
        std::stack<Octree*> ancestors;
        while (ptr != top) {
            ancestors.push(ptr);
            ptr = ptr->parent;
        }

        while (ptr != bottom) {
            bool found = false;
            for (size_t j = 0; j < 8; j++) {
                if (&(ptr->getChild(j)) == ancestors.top())
                {
                    ancestors.pop();
                    ptr = &(ptr->getChild(j));
                    found = true;

                    break;
                }
            }

            ASSERT_TRUE(found);
        }

        ASSERT_TRUE(ancestors.empty());
    }
}
