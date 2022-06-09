//
// Created by knyaz on 6/8/2022.
//
// Controller of the entire octree with some root-specific functions
//

#ifndef SABT_OCTREE_ROOT_H
#define SABT_OCTREE_ROOT_H

#include <Octree.h>

class OctreeRoot : public Octree {
protected:
    unsigned long long size;

public:
    OctreeRoot();

    ///< Scales the octree 2 times by each axis while origin remains in place
    void grow();

    ///< Returns the linear size of octree in minimal units
    unsigned long long getSize();
};


#endif //SABT_OCTREE_ROOT_H
