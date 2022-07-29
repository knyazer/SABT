//
// Created by knyaz on 20/07/22.
//

#include "include/OctreeBase.h"

OctreeBase::OctreeBase() {
    parent = nullptr;
}

void OctreeBase::fosterBy(OctreeBase *node, Triplet pos) {
    parent = node;
    tri = pos;
}

OctreeBase* OctreeBase::getRoot() const {
    OctreeBase* node = const_cast<OctreeBase *>(this);

    while (node->parent != nullptr) {
        node = node->parent;
    }

    return node;
}