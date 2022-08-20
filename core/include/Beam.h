//
// Created by knyaz on 25/07/22.
//

#ifndef SABT_BEAM_H
#define SABT_BEAM_H

#include "Vec3f.h"
#include "Shape3d.h"

#include <iostream>

/**
 * 3-dimensional shape which represents an infinite beam with 4 edges.
 * Consists of origin - point where the beam starts, and rays - points somewhere in the infinity,
 * corresponding to each edge of the beam.
 */
class Beam : public Shape3d {
public:
    Vec3f origin, rays[4];

    Beam(Vec3f origin, Vec3f *rays);

    Beam();

    void set(Vec3f origin, Vec3f *rays);

    /// Support function
    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f direction) const override;

    /// MPR wants it, currently return just the origin, as it is kinda the only point which makes sense
    [[nodiscard]] Vec3f getCenter() const override;

    /// Checks whether given point lies inside the beam
    [[nodiscard]] bool pointLiesInside(const Vec3f& point) const;
};


#endif //SABT_BEAM_H
