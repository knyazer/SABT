//
// Created by knyaz on 20/07/22.
//

#include "include/OctreeUnit.h"

OctreeUnit::OctreeUnit() {
    color = BLACK;
    exist = false;
}

bool OctreeUnit::isEmpty() {
    return !exist;
}

bool OctreeUnit::isFull() {
    return exist;
}

bool OctreeUnit::isSemi() {
    return false;
}

void OctreeUnit::fill() {
    exist = true;
}

void OctreeUnit::clear() {
    exist = false;
}

Color OctreeUnit::getColor(int faceIndex) {
    return color;
}

OctreeBase *OctreeUnit::getChild(Triplet tri) {
    return nullptr;
}

void OctreeUnit::setFilling(Filling filling) {
}

