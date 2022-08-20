//
// Created by knyaz on 6/11/2022.
//

#ifndef SABT_VEC3F_H
#define SABT_VEC3F_H

#include "Vec3i.h"
#include "BasicMath.h"

#include <cmath>
#include <iostream>

class Vec3f {
public:

    double x, y, z;

    Vec3f();

    Vec3f(const Vec3f& other);

    explicit Vec3f(Vec3i vec3I);

    Vec3f(double x, double y, double z);

    /// Subtract vectors elementwise
    Vec3f operator-(Vec3f other) const;

    /// Sum vectors elementwise
    Vec3f operator+(Vec3f other) const;

    /// Sum vector with scalar - add scalar to each dimension
    Vec3f operator+(double other) const;

    /// Scale
    Vec3f operator*(double other) const;

    /// Returns size of the vector
    [[nodiscard]] double size() const;

    /// Returns squared size of the vector, equivalent to dot(*this, *this)
    [[nodiscard]] double sqsize() const;

    /// Divides vector by its size; Normalization
    Vec3f norm() const;

    /// Cross product of two given vectors
    static Vec3f cross(const Vec3f& a, const Vec3f& b);

    /// Dot product of two given vectors
    static double dot(const Vec3f& a, const Vec3f& b);

    /// Elementwise min of two vectors
    static Vec3f min(const Vec3f& a, const Vec3f& b);

    /// Elementwise max of two vectors
    static Vec3f max(const Vec3f& a, const Vec3f& b);

    friend std::ostream& operator<<(std::ostream& os, const Vec3f& val);
};


#endif //SABT_VEC3F_H
