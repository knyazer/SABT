//
// Created by knyaz on 20/08/22.
//

#ifndef SABT_POINT3D_H
#define SABT_POINT3D_H

#include "Shape3d.h"
#include "Vec3f.h"

class Point3D : public Vec3f, public Shape3d {
public:

    Point3D() = default;

    explicit Point3D(const Vec3f& other) {
        x = other.x;
        y = other.y;
        z = other.z;
    }

    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f direction) const override {
        return *this;
    };

    [[nodiscard]] Vec3f getCenter() const override {
        return *this;
    }
};


#endif //SABT_POINT3D_H
