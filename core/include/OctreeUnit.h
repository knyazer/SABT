//
// Created by knyaz on 20/07/22.
//

#ifndef SABT_OCTREE_UNIT_H
#define SABT_OCTREE_UNIT_H

#include "OctreeBase.h"
#include "Color.h"

#include <cstdint>

/*
 * Lowest (smallest) nodes in octree.
 * These nodes take the least amount of memory, as they do not need to
 * contain all the children, and so they have only their state (color), existence flag (exist),
 * relative location (Triplet) and parent pointer.
 */
class OctreeUnit : public OctreeBase {
protected:
    // Does node exist? Makes sense only for the lowest level of octree
    bool exist;

public:
    // State of the unit node, for now just color
    Color color;

    OctreeUnit();

    // if node exists - false, otherwise - true
    bool isEmpty() override;

    // If node exists - true, otherwise - false
    bool isFull() override;

    // Always false for the unit node
    bool isSemi() override;

    // Sets the color(state) of the unit node
    void fill(Color color) override;

    // Remove the node from existence, simply sets existence flag to false
    void clear() override;

    // Getter for the state
    Color getColor(int faceIndex) override;
};


#endif //SABT_OCTREE_UNIT_H
