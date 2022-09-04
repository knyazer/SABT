//
// Created by knyaz on 31/08/22.
//

#ifndef SABT_EXPLICIT_INTERSECTIONS_H
#define SABT_EXPLICIT_INTERSECTIONS_H

#include "Beam.h"
#include "Cube.h"
#include "Vec3f.h"
#include "Vec2f.h"
#include "Shape2d.h"
#include "GJK.h"

namespace ExplicitIntersections {
    bool BeamCubeIntersection(Beam* beam, Cube* cube);
}

#endif //SABT_EXPLICIT_INTERSECTIONS_H
