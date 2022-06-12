//
// Created by knyaz on 6/11/2022.
//

#ifndef SABT_GEOMETRY_H
#define SABT_GEOMETRY_H

#include <stdexcept>
#include <iostream>

#include <Vec3f.h>
#include <Vec2f.h>

///< 3-dimensional line in a format of pos(t) = origin + t * direction
struct Line3d {
public:
    Vec3f origin;
    Vec3f direction;

    Line3d(Vec3f lineOrigin, Vec3f lineDirection);
};

struct Line2d {
public:
    Vec2f origin;
    Vec2f direction;

    Line2d(Vec2f lineOrigin, Vec2f lineDirection);
};

struct Unit3f {
    uint8_t index;

    Vec3f vector() {
        if (index == 0)
            return {1, 0, 0};

        if (index == 1)
            return {0, 1, 0};

        if (index == 2)
            return {0, 0, 1};

        throw std::runtime_error("Not allowed indices of unit besides 0(x), 1(y), 2(z).");
    }

    Vec3f setIndex(uint8_t newIndex) {
        index = newIndex;
        if (newIndex > 2)
            throw std::runtime_error("Not allowed indices of unit besides 0, 1, 2 .");
    }

    Unit3f() {
        index = 0;
    }

    Unit3f(uint8_t newIndex) {
        setIndex(newIndex);
    }
};

///< Plane in a format:    n . position = d
struct DoubleProjPlane {
public:
    // Normal to the given rect
    Unit3f n;
    double d1, d2;

    // Center of the given cube (rect)
    Vec3f origin;

    // Unit vectors
    Vec3f i, j;

    //
    //    third
    //      |
    //    corner
    //   /      \
    // first   second
    //
    DoubleProjPlane(Vec3f corner, Vec3f first, Vec3f second, Vec3f third) {
        origin = (first + second) * 0.5;
        i = (first - origin).norm(); //TODO: think about how to do that without norm
        j = (second - origin).norm();

        // We can determine the plane normal direction using the coordinate which is the same for all the points given
        if (origin.x == first.x)
            n = Unit3f(0);

        if (origin.y == first.y)
            n = Unit3f(1);

        if (origin.z == first.z)
            n = Unit3f(2);

        // Using the plane formula
        // TODO: Move to the ifs? check speedup
        d1 = Vec3f::dot(n.vector(), origin);
        d2 = Vec3f::dot(n.vector(), third);
    }

    ///< Projects any point to the plane coordinate system (the systems are the same for both d's)
    Vec2f project(Vec3f point) {
        // point = linear combination of i and j, excluding the normal index

        double a1, b1, c1, a2, b2, c2;
        if (n.index == 0) { // X coordinate to exclude
            // { i.y * A + j.y * B = point.y
            // { i.z * A + j.z * B = point.z

            a1 = i.y;
            b1 = j.y;
            c1 = point.y;

            a2 = i.z;
            b2 = j.z;
            c2 = point.z;
        }

        if (n.index == 1) { // Y coordinate to exclude
            // { i.x * A + j.x * B = point.x
            // { i.z * A + j.z * B = point.z

            a1 = i.x;
            b1 = j.x;
            c1 = point.x;

            a2 = i.z;
            b2 = j.z;
            c2 = point.z;
        }

        if (n.index == 2) { // Z coordinate to exclude
            // { i.y * A + j.y * B = point.y
            // { i.z * A + j.z * B = point.z

            a1 = i.y;
            b1 = j.y;
            c1 = point.y;

            a2 = i.z;
            b2 = j.z;
            c2 = point.z;
        }

        // Apply Cramer's rule to solve:

        double det = (a1 * b2 - b1 * a2);
        std::cout << "!det:" << det << std::endl; // TODO: check that det is constant and replace it everywhere

        if (det == 0)
            throw std::runtime_error("Determinant zero is not possible here, impossible situation, which means you fucked up somewhere");

        double A = (c1 * b2 - b1 * c2) / det;
        double B = (a1 * c2 - c1 * a2) / det;

        return {A, B};
    }

    Line2d project(Line3d line) {
        // It is possible to project a line projecting two points, and then building a line through them

        Vec2f first = project(line.origin), second = project(line.origin + line.direction);

        return {first, (second - first).norm()}; // TODO: Check whether there must be a norm
    }
};

double abs(double x) {
    return (x >= 0 ? x : -x);
}

bool lineIntersectsManhattansRect(Line2d line, double rectSize) {
    return (line.direction.x != 0 && (abs(line.origin.y - (line.origin.x / line.direction.x) * line.direction.y) <= rectSize)) ||
            (line.direction.y != 0 && (abs(line.origin.x - (line.origin.y / line.direction.y) * line.direction.x) <= rectSize));
}



#endif //SABT_GEOMETRY_H
