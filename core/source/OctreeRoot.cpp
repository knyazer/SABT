//
// Created by knyaz on 6/8/2022.
//

#include "OctreeRoot.h"

OctreeRoot::OctreeRoot() {
    logSize = 0;
    filling = EMPTY;
}

void OctreeRoot::grow() {
    logSize += 1;

    if (!hasChildren() || isEmpty())
        return;

    setFilling(SEMI);

    auto newChildren = std::vector<Octree>(8);

    for (size_t i = 0; i < 8; i++) {
        if (children[i].isEmpty()) {
            newChildren[i].setFilling(EMPTY);
        }
        else {
            newChildren[i].setFilling(SEMI);
            newChildren[i].makeChildren();
            newChildren[i].getChild(Triplet(i).reverse().index()) = children[i];
        }
    }

    children = newChildren;
}
// TODO: SEMI filling could be FULL actually
Octree* OctreeRoot::fill(Vec3i pos, unsigned level) {

    if (level == logSize) {
        Octree::fill();
        return this;
    }

    filling = SEMI;
    if (!hasChildren())
        makeChildren();

    Octree* node = &children[Triplet(pos).index()];
    Vec3i nextPos = pos;

    unsigned long long cubeSize = size();

    for (int i = 1; i < (logSize - level); i++) {
        unsigned long long half = cubeSize >> 1;

        // TODO: Make a special Vec3i functions like sign and +/-/etc implementations
        if (nextPos.x < 0)
            nextPos.x += half;
        if (nextPos.x >= 0)
            nextPos.x -= half; // TODO: check Vec sizeof
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

        if (node->isEmpty())
            node->setFilling(SEMI);

        if (!node->hasChildren())
            node->makeChildren();

        node = &node->getChild(Triplet(nextPos).index());

        cubeSize = half;
    }

    node->fill();

    return node;
}

unsigned long long OctreeRoot::size() const {
    return 1 << logSize;
}