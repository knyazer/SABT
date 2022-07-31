//
// Created by knyaz on 6/30/2022.
//

#ifndef SABT_SHAPE2D_H
#define SABT_SHAPE2D_H

#include "Vec2f.h"

#include <stdexcept>


/*
 * Base class for all 2-dimensional convex shapes
 *
 * Consists of one pure virtual function, which give the farthest point in a given direction.
 * That function is used in GJK support functions.
 */
class Shape2d {
public:
    [[nodiscard]] virtual Vec2f getFarthestPointInDirection(Vec2f direction) const = 0;
};


#endif //SABT_SHAPE2D_H
