//
// Created by knyaz on 6/8/2022.
//

#include "OctreeRoot.h"

OctreeRoot::OctreeRoot() {
    size = 1;
}

void OctreeRoot::grow() {
    size *= 2;

    if (!hasChildren())
        return;

    filling = SEMI;

    auto newChildren = std::make_shared<Octree[]>(8);

    for (size_t i = 0; i < 8; i++) {
        if (!children[i].isEmpty()) {
            newChildren[i].makeChildren();
        }
    }

    for (size_t i = 0; i < 8; i++) {
        if (newChildren[i].hasChildren()) {
            newChildren[i].getChild(Triplet(i).reverse().index()) = children[i];
            newChildren[i].setFilling(SEMI);
        }
    }

    children = newChildren;
}

unsigned long long OctreeRoot::getSize() {
    return size;
}