//
// Created by knyaz on 6/8/2022.
//
// Controller of the entire octree with some root-specific functions
//

#ifndef SABT_OCTREE_ROOT_H
#define SABT_OCTREE_ROOT_H

#include <Octree.h>
#include <iostream>
#include <vector>

class OctreeRoot : public Octree {
protected:
    unsigned long long logSize;

public:
    OctreeRoot();

    ///< Scales the octree 2 times by each axis while origin remains in place
    void grow();

    void fill(Vec3i pos, unsigned level);

    ///< Returns the linear size of octree in minimal units
    unsigned long long size() const;
};


#endif //SABT_OCTREE_ROOT_H
