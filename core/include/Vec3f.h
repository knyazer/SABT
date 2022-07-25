//
// Created by knyaz on 6/11/2022.
//

#ifndef SABT_VEC3F_H
#define SABT_VEC3F_H

#include <cmath>
#include <iostream>
#include "Vec3i.h"

class Vec3f {
public:

    double x, y, z;

    Vec3f();

    explicit Vec3f(Vec3i vec3I);

    Vec3f(double x, double y, double z);

    // Subtract vectors elementwise
    Vec3f operator-(Vec3f other);

    // Sum vectors elementwise
    Vec3f operator+(Vec3f other);

    // Sum vector with scalar - add scalar to each dimension
    Vec3f operator+(double other);

    // Scale
    Vec3f operator*(double other);

    // Returns size of the vector
    [[nodiscard]] double size() const;

    // Divides vector by its size; Normalization
    Vec3f norm();

    // Cross product of two given vectors
    static Vec3f cross(Vec3f a, Vec3f b);

    // Dot product of two given vectors
    static double dot(Vec3f a, Vec3f b);

    friend std::ostream& operator<<(std::ostream& os, const Vec3f& val);
};


#endif //SABT_VEC3F_H
