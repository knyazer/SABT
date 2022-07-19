//
// Created by knyaz on 6/8/2022.
//
// Controller of the entire octree with some root-specific functions
//

#ifndef SABT_OCTREE_ROOT_H
#define SABT_OCTREE_ROOT_H

#include <Octree.h>
#include <Cube.h>
#include <iostream>
#include <vector>
#include <stack>

using ll = long long;

class OctreeRoot : public Octree {
protected:
    unsigned long long logSize;

public:
    OctreeRoot();

    ///< Scales the octree 2 times by each axis while origin remains in place
    void grow();

    Octree* fill(Vec3i pos, unsigned level);

    ///< Returns the linear size of octree in minimal units
    ll size() const;

    Cube getCubeFor(Octree *node) const;
};


#endif //SABT_OCTREE_ROOT_H
