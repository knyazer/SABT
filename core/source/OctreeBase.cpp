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