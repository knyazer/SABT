//
// Created by knyaz on 6/8/2022.
//

#include "include/Octree.h"

Octree::Octree() {
    filling = EMPTY;
    parent = nullptr;
    children.clear();
}

OctreeBase *Octree::getChild(Triplet tri) {
    if (!hasChildren())
        throw std::runtime_error("Unable to access not initialized children of octree");
    return &children[tri.index()];
}

void Octree::makeChildren() {
    children = std::vector<Octree>(8);

    for (size_t i = 0; i < 8; i++) {
        children[i].fosterBy(this);
    }
}

bool Octree::hasChildren() {
    return !children.empty();
}

bool Octree::isEmpty() {
    return filling == EMPTY;
}

bool Octree::isFull() {
    return filling == FULL;
}

bool Octree::isSemi() {
    return filling == SEMI;
}

void Octree::fill() {
    filling = FULL;

    if (hasChildren()) {
        for (size_t i = 0; i < 8; i++) {
            children[i].fill();
        }
    }
}

void Octree::clear() {
    filling = EMPTY;

    if (hasChildren()) {
        for (size_t i = 0; i < 8; i++) {
            children[i].clear();
        }
    }
}

void Octree::deleteChildren() {
    if (hasChildren()) {
        for (size_t i = 0; i < 8; i++) {
            children[i].deleteChildren();
        }
    }

    children.clear();
}

void Octree::setFilling(Filling given) {
    filling = given;
}

Octree Octree::copy() {
    Octree clone;
    clone.fosterBy(parent);
    clone.setFilling(filling);
    clone.children = children;

    return clone;
}

Color Octree::getColor(int face) {
    return BLACK;
}
