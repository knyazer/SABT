//
// Created by knyaz on 6/12/2022.
//

#include "include/Cube.h"

Cube::Cube() {
    pos = Vec3i(0, 0, 0);
    size = 0;
}

Cube::Cube(Vec3i pos, ll size) {
    this->pos = pos;
    this->size = size;
}

Vec3i *Cube::getVertices() const {
    ll xm = pos.x + size, ym = pos.y + size, zm = pos.z + size;
    return new Vec3i[8]{
            pos,
            {pos.x, pos.y, zm},
            {pos.x, ym, pos.z},
            {pos.x, ym, zm},
            {xm, pos.y, pos.z},
            {xm, pos.y, zm},
            {xm, ym, pos.z},
            {xm, ym, zm}
    };
}

Vec3f Cube::getCenter() const {
    return Vec3f(pos) + static_cast<double>(size) * 0.5;
}

Vec3f Cube::getFarthestPointInDirection(Vec3f direction) const {
    return Vec3f(direction.x <= 0 ? pos.x : pos.x + size,
                 direction.y <= 0 ? pos.y : pos.y + size,
                 direction.z <= 0 ? pos.z : pos.z + size);
}

bool Cube::contain(Vec3i point) const {
    return  point.x >= pos.x && point.x <= pos.x + size &&
            point.y >= pos.y && point.y <= pos.y + size &&
            point.z >= pos.z && point.z <= pos.z + size;
}

bool Cube::cubeIntersectsCube(const Cube &A, const Cube &B) {
    Vec3i *verticesOfA = A.getVertices(), *verticesOfB = B.getVertices();
    for (size_t i = 0; i < 8; i++)
        if (B.contain(verticesOfA[i]) || A.contain(verticesOfB[i]))
            return true;

    return false;
}

std::ostream& operator<<(std::ostream& os, const Cube& val)
{
    os << "[Cube](" << "pos: " << val.pos << ", size: " << val.size << ")";
    return os;
}
