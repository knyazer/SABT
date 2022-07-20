//
// Created by knyaz on 6/30/2022.
//

#ifndef SABT_SHAPE2D_H
#define SABT_SHAPE2D_H

#include "Vec2f.h"

#include <stdexcept>

class Shape2d {
public:
    [[nodiscard]] virtual Vec2f getFarthestPointInDirection(Vec2f direction) const = 0;
};


#endif //SABT_SHAPE2D_H
