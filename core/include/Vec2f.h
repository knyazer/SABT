//
// Created by knyaz on 6/11/2022.
//

#ifndef SABT_VEC2F_H
#define SABT_VEC2F_H

#include <cmath>

class Vec2f {
public:
    double x, y;

    Vec2f();

    Vec2f(double newX, double newY);

    Vec2f operator-(Vec2f other);

    Vec2f operator*(double other);

    double size();

    Vec2f norm();
};


#endif //SABT_VEC2F_H
