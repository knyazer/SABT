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

    if (isEmpty())
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
        node = nodeMid->getChild(tri.index());

        cubeSize = half;
    }

    node->fill(color);

    return node;
}

ll OctreeRoot::size() const {
    return 1 << logSize;
}

Cube OctreeRoot::getCubeFor(OctreeBase *node) const {
    if (this == (OctreeRoot*)(node))
        return getCube();

    ll level = logSize;

    OctreeBase *ptr = node;
    while (ptr->parent != nullptr) {
        ptr = ptr->parent;
        level--;
    }

    ll delta = 1 << level;
    Vec3i pos;

    ptr = node;
    while (ptr->parent != nullptr) {
        pos = pos + (ptr->tri).vec() * delta;
        ptr = ptr->parent;
        delta <<= 1;
    }

    return {pos, 1 << level};
}

Cube OctreeRoot::getCube() const {
    return {{0, 0, 0}, size()};
}
