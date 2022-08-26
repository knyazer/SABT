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
    return children[tri.index()];
}

void Octree::makeChildren(bool makeUnit) {
    children = std::vector<OctreeBase *>(8);

    if (makeUnit) {
        for (size_t i = 0; i < 8; i++) {
            children[i] = new OctreeUnit();
        }
    } else {
        for (size_t i = 0; i < 8; i++) {
            children[i] = new Octree();
        }
    }

    for (size_t i = 0; i < 8; i++) {
        children[i]->fosterBy(this, i);
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

void Octree::fill(Color color) {
    filling = FULL;
    this->color = color;

    // TODO: spatial structure for each octree to store colors. Supposedly quadtree is good

    if (hasChildren())
        for (size_t i = 0; i < 8; i++)
            children[i]->fill(color);
}

void Octree::clear() {
    filling = EMPTY;

    if (hasChildren()) {
        for (size_t i = 0; i < 8; i++) {
            children[i]->clear();
        }
    }
}

void Octree::deleteChildren() {
    if (hasChildren())
        for (size_t i = 0; i < 8; i++)
            dynamic_cast<Octree *>(children[i])->deleteChildren();

    children.clear();
}

void Octree::setFilling(Filling given) {
    filling = given;
}

Octree Octree::copy() {
    Octree clone;
    clone.fosterBy(parent, tri);
    clone.setFilling(filling);
    clone.children = children;

    return clone;
}

Color Octree::getColor(int face) {
    return color;
}

Cube Octree::getCubeForChild(const Cube &rootCube, Triplet tri) {
    ll sz = rootCube.size / 2;
    return {Vec3i(tri.x(), tri.y(), tri.z()) * sz + rootCube.pos, sz};
}

Octree::~Octree() {
    children.clear();
}

void Octree::recomputeColor() {
    if (hasChildren()) {
        long long r = 0, g = 0, b = 0, numberOfNotEmptyChildren = 0;
        for (size_t i = 0; i < 8; i++) {
            if (children[i] != nullptr && !children[i]->isEmpty()) {

                auto *notUnitNode = dynamic_cast<Octree *>(children[i]);
                if (notUnitNode != nullptr)
                    notUnitNode->recomputeColor();

                r += children[i]->getColor(0).r;
                g += children[i]->getColor(0).g;
                b += children[i]->getColor(0).b;

                numberOfNotEmptyChildren++;
            }
        }

        color = {
                static_cast<ch_t>(r / numberOfNotEmptyChildren),
                static_cast<ch_t>(g / numberOfNotEmptyChildren),
                static_cast<ch_t>(b / numberOfNotEmptyChildren)
        };
    } else {
        color = Color::BLACK;
    }
}
