//
// Created by knyaz on 6/8/2022.
//

#include "include/Vec3i.h"

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

Vec3i Vec3i::operator*(long long factor) {
    return {x * factor, y * factor, z * factor};
}

Vec3i Vec3i::operator+(const Vec3i &other) {
    return {x + other.x, y + other.y, z + other.z};
}

Vec3i Vec3i::operator-(const Vec3i &other) {
    return {x - other.x, y - other.y, z - other.z};
}

Vec3i Vec3i::operator+(long long int value) {
    return Vec3i(x + value, y + value, z + value);
}

Vec3i::Vec3i(const Vec3i &other) {
    x = other.x;
    y = other.y;
    z = other.z;
}
