//
// Created by knyaz on 6/30/2022.
//

#ifndef SABT_GJK_H
#define SABT_GJK_H

#include "Vec2f.h"
#include <iostream>

/**
 * Namespace provides a bunch of functions which are needed for 2-dimensional convex intersection check.
 * The main one is GJK::GJK, which is checking whether there is an intersection. Code taken from my repo,
 * https://github.com/knyazer/RSS/blob/master/engine/include/core/GJK.hpp
 */
namespace GJK {

    const int ITERATIONS_LIMIT = 10;

    /// Return minkowski difference of passed shapes in a particular direction
    template<typename T1, typename T2>
    [[nodiscard]] Vec2f support(T1 A, T2 B, Vec2f direction)  {
        return A.getFarthestPointInDirection(direction) - B.getFarthestPointInDirection(direction.negate());
    }

    /// Checks whether 2 arbitrary convex shapes intersect
    template<typename T1, typename T2>
    [[nodiscard]] bool GJK(T1 A, T2 B, Vec2f d = {0.9338, -0.2333})  {
        size_t index = 0; // index of current vertex of simplex
        Vec2f a, b, c, ao, ab, ac, abPerp, acPerp, simplex[3];

        // if initial direction is zero – set it to any arbitrary direction
        if (d.x == 0 && d.y == 0) {
            d.x = -1.947;
            d.y = 0.4936;
        }

        // set the first support as initial point of the new simplex
        a = simplex[0] = support(A, B, d);

        if (Vec2f::dot(a, d) <= 0)
            return false; // no collision

        d = a.negate(); // The next search direction is always towards the origin, so the next search direction is negated

        uint64_t i = 0;
        while (true) {
            // If there are too many iterations - kill itself, generally means some precision problems
            if (i++ > ITERATIONS_LIMIT)
                return false;

            a = simplex[++index] = support(A, B, d);

            // Check whether $a lies on a separating axis
            if (Vec2f::dot(a, d) <= 0)
                return false; // no collision

            ao = a.negate(); // from point A to origin is just negative A

            // simplex has 2 points (a line segment, not a triangle yet)
            if (index < 2) {
                b = simplex[0];
                ab = b - a; // from point A to B
                d = Vec2f::tripleProduct(ab, ao, ab); // normal to AB towards Origin

                // if were unable to determine normal - choose something similar
                if (d.size() == 0)
                    d = ab.perp();

                // skip to next iteration
                continue;
            }

            b = simplex[1];
            c = simplex[0];
            ab = b - a; // from point A to B
            ac = c - a; // from point A to C

            acPerp = Vec2f::tripleProduct(ab, ac, ac);

            if (Vec2f::dot(acPerp, ao) >= 0) {
                d = acPerp; // new direction is normal to AC towards Origin

                // In case first line already passed through origin -> choose arbitrary direction
                if (d.x == 0 && d.y == 0) {
                    d.x = 0.122;
                    d.y = 1.389;
                }
            } else {
                abPerp = Vec2f::tripleProduct(ac, ab, ab);

                if (Vec2f::dot(abPerp, ao) < 0)
                    return true; // collision

                simplex[0] = simplex[1]; // swap first element (point C)

                d = abPerp; // new direction is normal to AB towards Origin
            }

            simplex[1] = simplex[2]; // swap element in the middle (point B)
            --index;
        }
    }
}
#endif //SABT_GJK_H
