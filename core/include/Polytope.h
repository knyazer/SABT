//
// Created by knyaz on 27/08/22.
//

#ifndef SABT_POLYTOPE_H
#define SABT_POLYTOPE_H

#include "Shape3d.h"
#include "Vec3f.h"

#include <vector>
#include <iostream>

/**
 * Represents polytope, a 3-dimensional convex volume defined by a set of points.
 */
class Polytope : public Shape3d {
protected:
    std::vector<Vec3f> vertices;

    Vec3f center;

public:

    Polytope() = default;

    Polytope(std::vector<Vec3f> _vertices);

    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f direction) const override;

    [[nodiscard]] Vec3f getCenter() const override;
};


#endif //SABT_POLYTOPE_H
