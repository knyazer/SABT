//
// Created by knyaz on 6/8/2022.
//

#include "include/OctreeRoot.h"

OctreeRoot::OctreeRoot() {
    logSize = 0;
    filling = EMPTY;
}

void OctreeRoot::grow() {
    logSize += 1;

    if (!hasChildren() || isEmpty())
        return;

    setFilling(SEMI);

    auto newChildren = std::vector<OctreeBase *>(8);
    for (size_t i = 0; i < 8; i++)
        newChildren[i] = new Octree();

    for (size_t i = 0; i < 8; i++) {
        auto* casted = dynamic_cast<Octree*>(newChildren[i]);

        if (children[i]->isEmpty()) {
            casted->setFilling(EMPTY);
        }
        else {
            casted->setFilling(SEMI);
            casted->makeChildren();
            *casted->getChild(Triplet(i).reverse().index()) = *children[i];
        }
    }

    children = newChildren;
}

// TODO: SEMI filling could be FULL actually
OctreeBase *OctreeRoot::fill(Vec3i pos, unsigned level, Color color) {
    if (pos.x < 0 || pos.y < 0 || pos.z < 0)
        throw std::runtime_error("Cannot fill node on negative coordinates: octree has only positive coords");

    if (pos.x >= size() || pos.y >= size() || pos.z >= size())
        throw std::runtime_error("Out of bounds node filling when calling fill at octree root");

    if (level == logSize) {
        Octree::fill(color);
        return this;
    }

    filling = SEMI;
    if (!hasChildren())
        makeChildren();

    OctreeBase *node = this;
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

        auto *nodeMid = dynamic_cast<Octree *>(node);

        if (nodeMid->isEmpty())
            nodeMid->setFilling(SEMI);

        if (!nodeMid->hasChildren()) {
            if (i == logSize - 1)
                nodeMid->makeChildren(MAKE_UNIT);
            else
                nodeMid->makeChildren();
        }

        Triplet tri(sign);
        node = dynamic_cast<Octree *>(node)->getChild(tri.index());

        cubeSize = half;
    }

    node->fill(color);

    return node;
}

ll OctreeRoot::size() const {
    return 1 << logSize;
}

Cube OctreeRoot::getCubeFor(OctreeBase *node) const {
    OctreeBase *ptr = node;
    std::stack<OctreeBase *> trace;
    while (ptr != this) {
        trace.push(ptr);
        ptr = ptr->parent;
    }

    Cube cube({0, 0, 0}, size());

    while (ptr != node) {
        bool found = false;
        for (size_t i = 0; i < 8; i++) {
            Triplet tri(i);
            OctreeBase *child = dynamic_cast<Octree *>(ptr)->getChild(i);

            if (child == trace.top()) {
                trace.pop();
                ptr = child;

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
