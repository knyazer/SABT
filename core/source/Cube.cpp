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

Vec3i* Cube::getVertices() {
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


