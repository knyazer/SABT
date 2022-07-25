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
    Vec3i *vertices = getVertices();
    auto vertex = Vec3f(vertices[0]);
    double maxProduct = Vec3f::dot(vertex, direction);

    for (size_t i = 1; i < 8; i++) {
        double dotProduct = Vec3f::dot(Vec3f(vertices[i]), direction);
        if (dotProduct > maxProduct) {
            maxProduct = dotProduct;
            vertex = Vec3f(vertices[i]);
        }
    }

    return vertex;
}

bool Cube::cubeIntersectsCube(const Cube &A, const Cube &B) {
    if (A.pos.x + A.size > B.pos.x && A.pos.x < B.pos.x + B.size)
        return true;
    if (A.pos.y + A.size > B.pos.y && A.pos.y < B.pos.y + B.size)
        return true;
    if (A.pos.z + A.size > B.pos.x && A.pos.z < B.pos.z + B.size)
        return true;

    return false;
}

