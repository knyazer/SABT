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
 * The type of the element contained in stack, currently made up from the pointer to the octree and its cube.
 */
struct OctreeAndCubePair {
    OctreeBase *node;
    Cube cube;

    OctreeAndCubePair() {
        node = nullptr;
    }

    OctreeAndCubePair(const OctreeAndCubePair &other) {
        node = other.node;
        cube = other.cube;
    }

    OctreeAndCubePair& operator=(const OctreeAndCubePair &other) {
        node = other.node;
        cube = other.cube;

        return *this;
    }

    OctreeAndCubePair(OctreeBase* _node, Cube _cube) : node(_node), cube(_cube) {};

    OctreeAndCubePair(OctreeBase* _node, Cube _cube, bool _certainIntersection) : node(_node), cube(_cube) {};

    void clearFromTempData() {

    }
};

/**
 * Represents a root (the topmost node) of the octree.
 * The whole octree is to be accessed from this root, as it provides
 * a bunch of checks and some other stuff to make life easier.
 */
class OctreeRoot : public Octree {
protected:
    /// log2 of the linear size of the octree
    unsigned long long logSize;

public:
    OctreeRoot();

    /// Scales the octree 2 times by each axis while origin remains in place
    void grow();

    /// Fill a particular node, defined by position and level, by the given color
    OctreeBase *fill(Vec3i pos, unsigned level, Color color);

    /// Returns the linear size of octree in minimal units
    ll size() const;

    /// Returns a series of nodes, which are successors to the first provided node, and ancestors of the second provided node in top-bottom order.
    /// Effectively provides an interface to manipulate with octree in any way, so kinda unsafe to use.
    /// Works only when second node is a successor of second one, otherwise throws an error.
    [[nodiscard]] std::vector<OctreeAndCubePair> constructDirectPathBetween(OctreeAndCubePair first, OctreeAndCubePair last) const;

    /// Returns a cube for a particular octree with pointer to it equals arg
    Cube getCubeFor(OctreeBase *node) const;

    /// Returns a cube for the root octree, is {{0, 0, 0}, 2^logSize}
    Cube getCube() const;

    /// Fits the provided mesh model into the octree
    void fitMesh(const Mesh& mesh, unsigned sz = 8);
};


#endif //SABT_OCTREE_ROOT_H
