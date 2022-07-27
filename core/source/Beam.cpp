//
// Created by knyaz on 25/07/22.
//

#include "include/Beam.h"

Beam::Beam(Vec3f origin, Vec3f* rays) {
    set(origin, rays);
}

void Beam::set(Vec3f origin, Vec3f* rays) {
    this->origin = origin;

    for (int i = 0; i < 4; i++)
        this->rays[i] = origin + rays[i] * 1000;
}

Vec3f Beam::getCenter() const {
    return origin;
}

Vec3f Beam::getFarthestPointInDirection(Vec3f direction) const {
    double maxProduct = Vec3f::dot(origin, direction);
    Vec3f res = origin;

    for (auto point : rays) { // TODO: dynamically detect thingy !imp
        double product = Vec3f::dot(point, direction);
        if (product > maxProduct) {
            maxProduct = product;
            res = point;
        }
    }

    return res;
}

Beam::Beam() {}
