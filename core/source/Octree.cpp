//
// Created by knyaz on 6/8/2022.
//

#include <Octree.h>
#include <iostream>

Octree::Octree() {
    filling = EMPTY;
    parent = nullptr;
    children.clear();
}

Octree &Octree::getChild(Triplet tri) {
    return children[tri.index()];
}

void Octree::makeChildren() {
    children = std::vector<Octree>(8);

    for (size_t i = 0; i < 8; i++) {
        children[i].foster(this);
    }
}

void Octree::foster(Octree* newParent) {
    parent = newParent;
}

bool Octree::hasChildren() {
    return !children.empty();
}

bool Octree::isEmpty() {
    return filling == EMPTY;
}

bool Octree::isFull() {
    return filling == FULL;
}

bool Octree::isSemi() {
    return filling == SEMI;
}

void Octree::fill() {
    filling = FULL;

    if (hasChildren()) {
        for (size_t i = 0; i < 8; i++) {
            children[i].fill();
        }
    }
}

void Octree::fill(Vec3i pos, unsigned long long size) {
    assert(__builtin_popcount(size) == 1 && "Size should be a power of 2");

    if (size == 1) {
        if (!isFull()) {
            filling = FULL;

            auto node = parent;
            while (node != nullptr) {
                assert(!node->isFull() && "Full node as a parent means something went wrong.");

                //TODO: speedup using the fact that if previous is SEMI then all parent also SEMI
                bool allNodesAreFull = true;
                for (size_t i = 0; i < 8; i++)
                    if (!(node->children[i]).isFull())
                        allNodesAreFull = false;

                if (allNodesAreFull)
                    node->filling = FULL;
                else
                    node->filling = SEMI;

                node = node->parent;
            }
        }
    } else {
        unsigned long long half = size >> 1;

        Vec3i nextPos = pos;

        //TODO: Make a special Vec3i functions like sign and +/-/etc implementations
        if (nextPos.x < 0)
            nextPos.x += half;
        if (nextPos.x >= 0)
            nextPos.x -= half;
        if (nextPos.y < 0)
            nextPos.y += half;
        if (nextPos.y >= 0)
            nextPos.y -= half;
        if (nextPos.z < 0)
            nextPos.z += half;
        if (nextPos.z >= 0)
            nextPos.z -= half;

        if (!hasChildren())
            makeChildren();

        children[Triplet(pos).index()].fill(nextPos, half);
    }
}

void Octree::clear() {
    filling = EMPTY;

    if (hasChildren()) {
        for (size_t i = 0; i < 8; i++) {
            children[i].clear();
        }
    }
}

void Octree::deleteChildren() {
    if (hasChildren()) {
        for (size_t i = 0; i < 8; i++) {
            children[i].deleteChildren();
        }
    }

    children.clear();
}

void Octree::setFilling(Filling given) {
    filling = given;
}