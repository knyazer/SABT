//
// Created by knyaz on 6/12/2022.
//

#ifndef SABT_CUBE_H
#define SABT_CUBE_H

#include "Vec3i.h"
#include "Shape3d.h"

#include <vector>


using ll = long long;

/**
 * 3-dimensional axis aligned cube with integer position and size.
 */
struct Cube : public Shape3d {
    Vec3i pos;
    ll size;

    Cube();

    Cube(const Vec3i &newPos, ll newSize);

    /// Checks whether a particular point contained in the Rect
    [[nodiscard]] bool contain(Vec3i point) const;

    /// Returns 8 vertices of the cube in arbitrary order
    [[nodiscard]] Vec3i *getVertices() const;

    /// Returns the center of the current cube -> (pos + size / 2)
    [[nodiscard]] Vec3f getCenter() const override;

    /// MPR support function; returns farthest point in a given direction
    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f direction) const override;

    /// Checks whether two cubes intersect each other
    [[nodiscard]] static bool cubeIntersectsCube(const Cube &A, const Cube &B);

    friend std::ostream& operator<<(std::ostream& os, const Cube& val);
};


#endif //SABT_CUBE_H
