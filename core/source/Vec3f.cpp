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

Vec3f Vec3f::cross(Vec3f a, Vec3f b) {
    return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

double Vec3f::dot(Vec3f a, Vec3f b) {
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
    return operator*(1 / size());
}

std::ostream& operator<<(std::ostream& os, const Vec3f& val)
{
    os << "[Vec3f](" << val.x << ", " << val.y << ", " << val.z << ")";
    return os;
}
