//
// Created by knyaz on 20/07/22.
//

#ifndef SABT_OCTREE_BASE_H
#define SABT_OCTREE_BASE_H

#include "Color.h"
#include "Triplet.h"
#include "Filling.h"

const bool MAKE_UNIT = true;

class OctreeBase {
public:
    Triplet tri;

    OctreeBase *parent;

    OctreeBase();

    [[nodiscard]] OctreeBase* getRoot() const;

    virtual void fosterBy(OctreeBase *node, Triplet pos);

    virtual Color getColor(int faceIndex) = 0;

    virtual bool isEmpty() = 0;

    virtual bool isFull() = 0;

    virtual bool isSemi() = 0;

    virtual void fill(Color color) = 0;

    virtual void clear() = 0;

    friend std::ostream& operator<<(std::ostream& os, const OctreeBase& val);
};


#endif //SABT_OCTREE_BASE_H
