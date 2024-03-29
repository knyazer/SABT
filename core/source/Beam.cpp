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
        this->rays[i] = origin + rays[i] * 10000;
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

Beam::Beam() {

}

bool Beam::pointLiesInside(const Vec3f &point) const {

    for (size_t i = 0; i < 4; i++) {

        Vec3f normal = Vec3f::cross((rays[i] - origin), (rays[(i + 1) % 4] - origin));

        if (Vec3f::dot(point - origin, normal) <= 0)
            return false;
    }

    return true;
}

Polygon Beam::project(Vec3f::Index plane, double distance) const {
    std::vector<Vec2f> result(4);

    for (size_t i = 0; i < 4; i++) {
        Vec3f diff = rays[i] - origin;
        double factor = distance / diff[plane];

        if (factor < 0)
            return Polygon({{-100, 100}, {-100, -100}, {-100, -100}, {-100, 100}});

        result[i] = (diff * factor + origin).buildNormalPlane(plane);
    }

    return Polygon(result);
}
