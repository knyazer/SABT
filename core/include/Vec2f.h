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

    Vec2f operator+(Vec2f other) const;

    Vec2f operator-(Vec2f other) const;

    Vec2f operator*(double other) const;

    [[nodiscard]] double size() const;

    [[nodiscard]] Vec2f norm() const;

    [[nodiscard]] Vec2f perp() const;

    [[nodiscard]] Vec2f negate() const;

    static double dot(Vec2f a, Vec2f b);

    static Vec2f tripleProduct(Vec2f a, Vec2f b, Vec2f c);
};


#endif //SABT_VEC2F_H
