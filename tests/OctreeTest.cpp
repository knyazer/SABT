//
// Created by knyaz on 6/8/2022.
//

#include "gtest/gtest.h"

#include <SABT.h>

#include <stack>

TEST(Octree, SimpleFillingTest) {
    Octree octree;

    ASSERT_EQ(octree.isEmpty(), true);
    ASSERT_EQ(octree.isFull(), false);

    octree.fill(BLACK);

    ASSERT_EQ(octree.isEmpty(), false);
    ASSERT_EQ(octree.isFull(), true);

    octree.clear();

    ASSERT_EQ(octree.isEmpty(), true);
    ASSERT_EQ(octree.isFull(), false);
}

TEST(Octree, SingleGrowthTest) {
    OctreeRoot octree;

    octree.grow();
    octree.grow();

    octree.fill(Vec3i(0, 0, 0), 0, BLACK);

    octree.grow();
    octree.grow();

    OctreeBase* child = &octree;
    for (int i = 1; i <= 16; i *= 2) {
        if (i != 16) {
            ASSERT_EQ(child->isSemi(), true);
        } else {
            ASSERT_EQ(child->isFull(), true);
        }

        if (i == 1) {
            child = dynamic_cast<Octree*>(child)->getChild({1, 1, 1});
        } else if (i != 16) {
            child = dynamic_cast<Octree*>(child)->getChild({0, 0, 0});
        }
    }
}

TEST(Octree, GetCubeTest) {
    OctreeRoot octree;

    for (size_t i = 0; i < 8; i++)
        octree.grow();

    for (size_t i = 0; i < 100; i++) {
        int x = rand() % 256, y = rand() % 256, z = rand() % 256;
        int level = 0;

        for (size_t j = 0; j < level; j++) {
            x = (x / 2) * 2;
            y = (y / 2) * 2;
            z = (z / 2) * 2;
        }

        OctreeBase *node = octree.fill({x, y, z}, level, BLACK);
        Cube cube = octree.getCubeFor(node);

        ASSERT_TRUE(cube.pos.x == x && cube.pos.y == y && cube.pos.z == z);
        ASSERT_EQ(cube.size, 1 << level);
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

        OctreeBase* bottom = octree.fill({x, y, z}, level, BLACK);
        OctreeBase* top = &octree;

        // Test that ancestors with top->bottom are the same as with bottom->top
        OctreeBase* ptr = bottom;
        std::stack<OctreeBase*> ancestors;
        while (ptr != top) {
            ancestors.push(ptr);
            ptr = ptr->parent;
        }

        while (ptr != bottom) {
            bool found = false;
            for (size_t j = 0; j < 8; j++) {
                if (dynamic_cast<Octree*>(ptr)->getChild(j) == ancestors.top())
                {
                    ancestors.pop();
                    ptr = dynamic_cast<Octree*>(ptr)->getChild(j);
                    found = true;

                    break;
                }
            }

            ASSERT_TRUE(found);
        }

        ASSERT_TRUE(ancestors.empty());
    }
}
