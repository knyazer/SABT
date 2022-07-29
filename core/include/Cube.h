//
// Created by knyaz on 6/12/2022.
//

#ifndef SABT_CUBE_H
#define SABT_CUBE_H

#include "Vec3i.h"
#include "Shape3d.h"

#include <vector>


using ll = long long;

struct Cube : public Shape3d {
    Vec3i pos;
    ll size;

    Cube();

    Cube(Vec3i pos, ll size);

    [[nodiscard]] bool contain(Vec3i point) const;

    [[nodiscard]] Vec3i *getVertices() const;

    [[nodiscard]] Vec3f getCenter() const override;

    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f direction) const override;

    [[nodiscard]] static bool cubeIntersectsCube(const Cube &A, const Cube &B);

    friend std::ostream& operator<<(std::ostream& os, const Cube& val);
};


#endif //SABT_CUBE_H
