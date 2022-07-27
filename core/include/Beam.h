//
// Created by knyaz on 25/07/22.
//

#ifndef SABT_BEAM_H
#define SABT_BEAM_H

#include "Vec3f.h"
#include "Shape3d.h"

#include <iostream>

class Beam : public Shape3d {
public:
    Vec3f origin, rays[4];

    Beam(Vec3f origin, Vec3f *rays);

    Beam();

    void set(Vec3f origin, Vec3f *rays);

    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f direction) const override;

    [[nodiscard]] Vec3f getCenter() const override;
};


#endif //SABT_BEAM_H
