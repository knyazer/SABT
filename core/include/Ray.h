//
// Created by knyaz on 12/08/22.
//

#ifndef SABT_RAY_H
#define SABT_RAY_H

#include "Vec3f.h"
#include "Cube.h"
#include "BasicMath.h"

/**
 * 3D Ray. Used in a final stage of rendering to find the exact nodes corresponding to the pixel.
 * Supports both usage with MPR and with specific intersection algorithm for rays and cubes.
 */
class Ray : public Shape3d {
protected:

    Vec3f origin, direction;

public:

    Ray(Vec3f origin, Vec3f direction);

    /// Quickly finds intersection between provided cube and the ray
    [[nodiscard]] bool hasIntersection(Cube cube) const;

    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f projDirection) const;

    [[nodiscard]] Vec3f getCenter() const;
};


#endif //SABT_RAY_H
