//
// Created by knyaz on 6/4/2022.
//


#ifndef SABT_OCTREE_H
#define SABT_OCTREE_H

#include "Triplet.h"
#include "Filling.h"
#include "Vec3i.h"
#include "Cube.h"
#include "OctreeBase.h"
#include "OctreeUnit.h"

#include <memory>
#include <cstddef>
#include <vector>

using std::size_t;

/**
 * Main class for the octree node
 * Each octree has 8 children, each of them is octree
 * Each octree has a backlink to the parent for the inverse traversal
 * Each octree has three states for the filling (Filling.h)
 * Each octree has an alix aligned bounding box which is defined by parent octree, and it's index (triplet).
 */
class Octree : public std::enable_shared_from_this<Octree>, public OctreeBase {
protected:
    std::vector<OctreeBase*> children;

    Color color;

    Filling filling;

public:

    Octree();

    /// Generate new children for the current node
    void makeChildren(bool makeUnit = false);

    /// Get child by the given triplet
    OctreeBase *getChild(Triplet tri);

    /// Does the tree has any children
    bool hasChildren();

    /// Is the tree empty
    bool isEmpty() override;

    /// Is the tree full
    bool isFull() override;

    /// Is the tree filled only partly
    bool isSemi() override;

    /// Fill the octree; recursively fills (sets filling to FULL) all the children
    void fill(Color color) override;

    /// Empty (or clear) the octree; recursively sets the filling to EMPTY
    void clear() override;

    /// Recursively deletes all the children of the octree
    void deleteChildren();

    /// Sets the filling of the only current node. Better not to use, kept it for the backwards compatibility
    void setFilling(Filling given);

    /// Gets the color of a particular face
    Color getColor(int face) override;

    /// Returns the cube of the child from the current cube.
    /// Equivalent to {pos + tri.vec * size / 2, size / 2)
    static Cube getCubeForChild(const Cube &rootCube, Triplet tri);

    /// Copies the current octree
    Octree copy();

    ~Octree();
};

#endif //SABT_OCTREE_H
