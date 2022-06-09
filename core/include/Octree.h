//
// Created by knyaz on 6/4/2022.
//
// Main class for the octree node
// Each octree has 8 children, each of them is octree
// Each octree has a backlink to the parent for the inverse traversal
// Each octree has three states for the filling (Filling.h)
// Each octree has an alix aligned bounding box which is defined by parent octree, and it's index (triplet).
//

#ifndef SABT_OCTREE_H
#define SABT_OCTREE_H

#include <Triplet.h>
#include <Filling.h>
#include <Vec3i.h>
#include <memory>

///< Main class for the octree administration
class Octree : public std::enable_shared_from_this<Octree> {
protected:
    std::shared_ptr<Octree[]> children;
    Octree* parent;

    Filling filling;

public:
    Octree();

    ///< Generate new children for the current node
    void makeChildren();

    ///< Get child by the given triplet
    Octree& getChild(Triplet tri);

    ///< Does the tree has any children
    bool hasChildren();

    ///< Is the tree empty
    bool isEmpty();

    ///< Is the tree full
    bool isFull();

    ///< Is the tree filled only partly
    bool isSemi();

    ///< Sets the parent of the octree
    void foster(Octree* parent);

    ///< Fill the octree; recursively fills (sets filling to FULL) all the children
    void fill();

    ///< Recursively fill a particular node at a given position
    void fill(Vec3i nextPos, unsigned long long size);

    ///< Empty (or clear) the octree; recursively sets the filling to EMPTY
    void clear();

    ///< Recursively deletes all the children of the octree
    void deleteChildren();

    ///< Sets the filling of the only current node. Better not to use, kept it for the simplicity of development
    void setFilling(Filling given);
};

#endif //SABT_OCTREE_H
