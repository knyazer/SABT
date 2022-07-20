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

std::vector<Vec3i> Cube::getVertices() {
    return {
            pos,
            pos + Vec3i(0, 0, size),
            pos + Vec3i(0, size, 0),
            pos + Vec3i(0, size, size),
            pos + Vec3i(size, 0, 0),
            pos + Vec3i(size, 0, size),
            pos + Vec3i(size, size, 0),
            pos + Vec3i(size, size, size)
    };
}


