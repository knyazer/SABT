//
// Created by knyaz on 31/08/22.
//

#include "include/ExplicitIntersections.h"

bool ExplicitIntersections::BeamCubeIntersection(Beam *beam, Cube *cube) {

    if (cube->contain(beam->origin))
        return true;

    std::vector<Vec3f::Index> indices = {Vec3f::Index::X, Vec3f::Index::Y, Vec3f::Index::Z};
    for (char i = 0; i < 3; i++) {
        auto mainDirection = Vec3f::Index(i); // 0 - x, 1 - y, 2 - z
        // mainDirection = i;
        double dst = cube->getCenter()[mainDirection] - beam->origin[mainDirection];

        auto polyFarthest = beam->project(mainDirection, dst + cube->getHalfSize() * sign(dst));
        if (cube->getFaceInDirection(mainDirection).intersects(polyFarthest.makeBB()))
            if (GJK::GJK(polyFarthest, cube->getFaceInDirection(mainDirection)))
                return true;

        auto polyClosest = beam->project(mainDirection, dst - cube->getHalfSize() * sign(dst));
        if (cube->getFaceInDirection(mainDirection).intersects(polyClosest.makeBB()))
            if (GJK::GJK(polyClosest, cube->getFaceInDirection(mainDirection)))
                return true;
    }

    return false;
}