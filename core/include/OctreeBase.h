//
// Created by knyaz on 20/07/22.
//

#ifndef SABT_OCTREE_BASE_H
#define SABT_OCTREE_BASE_H

#include "Color.h"
#include "Triplet.h"
#include "Filling.h"

#include <memory>

using std::shared_ptr;

class OctreeBase {
public:
    OctreeBase();

    OctreeBase *parent;

    virtual void fosterBy(OctreeBase *parent);

    virtual void makeChildren() = 0;

    virtual bool hasChildren() = 0;

    virtual Color getColor(int faceIndex) = 0;

    virtual OctreeBase* getChild(Triplet tri) = 0;

    virtual void deleteChildren() = 0;

    virtual bool isEmpty() = 0;

    virtual bool isFull() = 0;

    virtual bool isSemi() = 0;

    virtual void fill() = 0;

    virtual void clear() = 0;

    virtual void setFilling(Filling filling) = 0;
};


#endif //SABT_OCTREE_BASE_H
