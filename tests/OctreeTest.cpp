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

    octree.fill(Color::BLACK);

    ASSERT_EQ(octree.isEmpty(), false);
    ASSERT_EQ(octree.isFull(), true);

    octree.clear();

    ASSERT_EQ(octree.isEmpty(), true);
    ASSERT_EQ(octree.isFull(), false);
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

        OctreeBase *node = octree.fill({x, y, z}, level, Color::BLACK);
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
        int x = rand() % 7, y = rand() % 9, z = rand() % 11;
        int level = rand() % 3;

        for (size_t j = 0; j < level; j++) {
            x = (x / 2) * 2;
            y = (y / 2) * 2;
            z = (z / 2) * 2;
        }
        
        OctreeBase* bottom = octree.fill({x, y, z}, level, Color::BLACK);
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
