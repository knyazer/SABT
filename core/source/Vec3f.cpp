//
// Created by knyaz on 6/11/2022.
//

#include "include/Vec3f.h"


Vec3f::Vec3f() {
    x = 0;
    y = 0;
    z = 0;
}

Vec3f::Vec3f(Vec3i vec) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

Vec3f::Vec3f(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3f Vec3f::cross(const Vec3f& a, const Vec3f& b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

double Vec3f::dot(const Vec3f& a, const Vec3f& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3f Vec3f::operator-(Vec3f other) const {
    return {x - other.x, y - other.y, z - other.z};
}

Vec3f Vec3f::operator*(double other) const {
    return {x * other, y * other, z * other};
}

Vec3f Vec3f::operator+(Vec3f other) const {
    return {x + other.x, y + other.y, z + other.z};
}

Vec3f Vec3f::operator+(double other) const {
    return {x + other, y + other, z + other};
}

double Vec3f::size() const {
    return sqrt(x * x + y * y + z * z);
}

Vec3f Vec3f::norm() const {
    double sz = size();

    if (sz != 0)
        return operator*(1 / size());

    return {0, 0, 0};
}

Vec3f::Vec3f(const Vec3f &other) {
    x = other.x;
    y = other.y;
    z = other.z;
}

Vec3f Vec3f::min(const Vec3f &a, const Vec3f &b) {
    return {min2(a.x, b.x), min2(a.y, b.y), min2(a.z, b.z)};
}

Vec3f Vec3f::max(const Vec3f &a, const Vec3f &b) {
    return {max2(a.x, b.x), max2(a.y, b.y), max2(a.z, b.z)};
}

double Vec3f::sqsize() const {
    return x * x + y * y + z * z;
}

Vec3f::Index Vec3f::getMainDirection() const {
    // Omitting useless else's

    if (abs(x) >= abs(y)) {
        if (abs(x) >= abs(z))
            return X;
        return Z;
    }

    if (abs(y) >= abs(z))
        return Y;

    return Z;
}

double &Vec3f::operator[](Vec3f::Index index) {
    if (index == X)
        return x;
    if (index == Y)
        return y;
    return z;
}

Vec2f Vec3f::buildNormalPlane(Vec3f::Index index) const {
    if (index == X)
        return {y, z};
    if (index == Y)
        return {x, z};
    return {x, y};
}

Vec3f::Vec3f(Vec3f::Index i) {
    x = 0;
    y = 0;
    z = 0;

    if (i == X)
        x = 1;
    if (i == Y)
        y = 1;
    if (i == Z)
        z = 1;
}
