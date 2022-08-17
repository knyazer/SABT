//
// Created by knyaz on 12/08/22.
//

#include "include/Ray.h"

Ray::Ray(Vec3f origin, Vec3f direction) {
    this->origin = origin;
    this->direction = direction.norm();
}

bool Ray::hasIntersection(Cube cube) const {
    double t;
    Vec3f dirfrac;

    Vec3i lb = cube.pos;
    Vec3i rt = cube.pos + cube.size;

    // Make sure that there is no division by zero
    dirfrac.x = 1.0 / (abs(direction.x) <= 1e-10 ? 1e-10 * sign(direction.x) : direction.x);
    dirfrac.y = 1.0 / (abs(direction.y) <= 1e-10 ? 1e-10 * sign(direction.y) : direction.y);
    dirfrac.z = 1.0 / (abs(direction.z) <= 1e-10 ? 1e-10 * sign(direction.z) : direction.z);

    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    double t1 = (lb.x - origin.x) * dirfrac.x;
    double t2 = (rt.x - origin.x) * dirfrac.x;
    double t3 = (lb.y - origin.y) * dirfrac.y;
    double t4 = (rt.y - origin.y) * dirfrac.y;
    double t5 = (lb.z - origin.z) * dirfrac.z;
    double t6 = (rt.z - origin.z) * dirfrac.z;

    double tmin = max2(max2(min2(t1, t2), min2(t3, t4)), min2(t5, t6));
    double tmax = min2(min2(max2(t1, t2), max2(t3, t4)), max2(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
    // if tmin > tmax ray does not intersect AABB
    if (tmax < 0 || tmin > tmax)
        return false;

    return true;
}

Vec3f Ray::getCenter() const {
    return origin;
}

Vec3f Ray::getFarthestPointInDirection(Vec3f projDirection) const {
    if (Vec3f::dot(origin, projDirection) > Vec3f::dot(origin + direction * 100000, projDirection))
        return origin;

    return origin + direction * 100000; // TODO: Fix this thing
}

Vec3f Ray::getDirection() const {
    return direction;
}
