//
// Created by knyaz on 29/07/22.
//

#include "../SABT.h"

std::ostream &operator<<(std::ostream &os, const OctreeBase &val) {
    OctreeRoot *root = dynamic_cast<OctreeRoot *>(val.getRoot());

    os << "[Octree](" << "cube: " << root->getCubeFor((OctreeBase *) &val) << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Vec2f &val) {
    os << "[Vec2f](" << val.x << ", " << val.y << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Vec3f &val) {
    os << "[Vec3f](" << val.x << ", " << val.y << ", " << val.z << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Vec3i &val) {
    os << "[Vec3i](" << val.x << ", " << val.y << ", " << val.z << ")";
    return os;
}


std::ostream &operator<<(std::ostream &os, const Triplet &val) {
    os << "[Triplet](" << val.x() << ", " << val.y() << ", " << val.z() << ")";
    return os;
}

std::ostream &operator<<(std::ostream &os, const Biplet &val) {
    os << "[Biplet](" << val.x() << ", " << val.y() << ")";
    return os;
}