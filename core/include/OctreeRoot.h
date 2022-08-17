//
// Created by knyaz on 6/8/2022.
//

#ifndef SABT_OCTREE_ROOT_H
#define SABT_OCTREE_ROOT_H

#include "Octree.h"
#include "Cube.h"
#include "Color.h"
#include "Mesh.h"
#include "BoundingBox.h"

#include <iostream>
#include <vector>
#include <stack>

using ll = long long;

/**
 * Represents a root (the topmost node) of the octree.
 * The whole octree is to be accessed from this root, as it provides
 * a bunch of checks and some other stuff to make life easier.
 */
class OctreeRoot : public Octree {
protected:
    /// log2 of the real-world size of the octree
    unsigned long long logSize;

public:
    OctreeRoot();

    /// Scales the octree 2 times by each axis while origin remains in place
    void grow();

    /// Fill a particular node, defined by position and level, by the given color
    OctreeBase *fill(Vec3i pos, unsigned level, Color color);

    /// Returns the linear size of octree in minimal units
    ll size() const;

    /// Returns a cube for a particular octree with pointer to it equals arg
    Cube getCubeFor(OctreeBase *node) const;

    /// Returns a cube for the root octree, is {{0, 0, 0}, 2^logSize}
    Cube getCube() const;

    /// Fits the provided mesh model into the octree
    void fitMesh(const Mesh& mesh, unsigned sz = 8);
};


#endif //SABT_OCTREE_ROOT_H
