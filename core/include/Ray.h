//
// Created by knyaz on 12/08/22.
//

#ifndef SABT_RAY_H
#define SABT_RAY_H

#include "Vec3f.h"
#include "Cube.h"
#include "BasicMath.h"

class Ray : public Shape3d {
protected:

    Vec3f origin, direction;

public:

    Ray(Vec3f origin, Vec3f direction);

    [[nodiscard]] bool hasIntersection(Cube cube) const;

    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f projDirection) const;

    [[nodiscard]] Vec3f getCenter() const;
};


#endif //SABT_RAY_H
