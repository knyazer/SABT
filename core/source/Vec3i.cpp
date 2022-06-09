//
// Created by knyaz on 6/8/2022.
//

#include "Vec3i.h"

Vec3i::Vec3i() {
    x = 0;
    y = 0;
    z = 0;
}

Vec3i::Vec3i(long long x, long long y, long long z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

const Vec3i Vec3i::operator*(long long factor) {
    return {x * factor, y * factor, z * factor};
}