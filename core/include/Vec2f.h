//
// Created by knyaz on 6/11/2022.
//

#ifndef SABT_VEC2F_H
#define SABT_VEC2F_H

#include <cmath>
#include <iostream>

class Vec2f {
public:
    double x, y;

    Vec2f();

    Vec2f(double newX, double newY);

    Vec2f(const Vec2f& other);

    [[nodiscard]] bool operator==(Vec2f other) const;

    [[nodiscard]] Vec2f operator+(Vec2f other) const;

    [[nodiscard]] Vec2f operator-(Vec2f other) const;

    [[nodiscard]] Vec2f operator*(double other) const;

    [[nodiscard]] double size() const;

    [[nodiscard]] double sizesq() const;

    [[nodiscard]] Vec2f norm() const;

    [[nodiscard]] Vec2f perp() const;

    [[nodiscard]] Vec2f negate() const;

    [[nodiscard]] static double dot(Vec2f a, Vec2f b);

    [[nodiscard]] static Vec2f tripleProduct(Vec2f a, Vec2f b, Vec2f c);

    [[nodiscard]] double size_1() const;

    friend std::ostream& operator<<(std::ostream& os, const Vec2f& val);
};


#endif //SABT_VEC2F_H
