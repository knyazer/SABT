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
    if (pos.x < 0 || pos.y < 0 || pos.z < 0)
        throw std::runtime_error("Cannot fill node on negative coordinates: octree has only positive coords");

    if (pos.x >= size() || pos.y >= size() || pos.z >= size())
        throw std::runtime_error("Out of bounds node filling when calling fill at octree root");

    if (level == logSize) {
        Octree::fill();
        return this;
    }

    filling = SEMI;
    if (!hasChildren())
        makeChildren();

    Octree* node = this;
    Vec3i nextPos = pos;

    ll cubeSize = size();

    for (int i = 0; i < (logSize - level); i++) {
        ll half = cubeSize >> 1;

        // TODO: Make a special Vec3i functions like sign and +/-/etc implementations
        Vec3i sign;
        if (nextPos.x > 0 && nextPos.x >= half) sign.x = 1;
        else sign.x = 0;

        if (nextPos.y > 0 && nextPos.y >= half) sign.y = 1;
        else sign.y = 0;

        if (nextPos.z > 0 && nextPos.z >= half) sign.z = 1;
        else sign.z = 0;

        nextPos = nextPos - sign * half;

        if (node->isEmpty())
            node->setFilling(SEMI);

        if (!node->hasChildren())
            node->makeChildren();

        Triplet tri(sign);

        node = &node->getChild(tri.index());

        cubeSize = half;
    }

    node->fill();

    return node;
}

ll OctreeRoot::size() const {
    return 1 << logSize;
}

Cube OctreeRoot::getCubeFor(Octree *node) const {
    Octree* ptr = node;
    std::stack<Octree*> trace;
    while (ptr != this) {
        trace.push(ptr);
        ptr = ptr->parent;
    }

    Cube cube({0, 0, 0}, size());

    while (ptr != node) {
        bool found = false;
        for (size_t i = 0; i < 8; i++) {
            Triplet tri(i);
            if (&(ptr->getChild(i)) == trace.top())
            {
                trace.pop();
                ptr = &(ptr->getChild(i));

                cube.size /= 2;
                cube.pos = cube.pos + Vec3i(tri.x(), tri.y(), tri.z()) * cube.size;

                found = true;

                break;
            }
        }

        if (!found)
            throw std::runtime_error("The child with required address was not found");
    }

    return cube;
}
