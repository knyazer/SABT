//
// Created by knyaz on 20/07/22.
//

#ifndef SABT_OCTREE_BASE_H
#define SABT_OCTREE_BASE_H

#include "Color.h"
#include "Triplet.h"
#include "Filling.h"

const bool MAKE_UNIT = true;
const bool MAKE_NORMAL = false;

/**
 * Base class for all octree-related classes. Allows polymorphism.
 *
 * Consists of triplet, which stores the location relative to the top level node and parent pointer,
 * in total 3 + 16 = 19 bit, assuming the pointer is 16 bit. For now pointer is 64 bit, so the total
 * size is 67 bit, which is a lot, but whatever.
 *
 * Contains a bunch of pure virtual functions which are needed for the runtime polymorphism.
 */
class OctreeBase {
public:
    /// Stores the location relative to the parent node
    Triplet tri;

    /// Pointer to the parent of the current node. By default, equals to nullptr.
    OctreeBase *parent;

    OctreeBase();

    /// Return the topmost element in the octree. Moves upwards until parent pointer is not nullptr.
    [[nodiscard]] OctreeBase* getRoot() const;

    /// Set the parent, and this node location relative to it
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
