//
// Created by knyaz on 6/11/2022.
//

#include "Vec2f.h"

Vec2f::Vec2f(double newX, double newY) {
    x = newX;
    y = newY;
}

Vec2f::Vec2f() {
    x = 0;
    y = 0;
}

Vec2f Vec2f::norm() {
    return operator*(1 / size());
}

Vec2f Vec2f::operator-(Vec2f other) {
    return {x - other.x, y - other.y};
}

Vec2f Vec2f::operator*(double other) {
    return {x * other, y * other};
}

double Vec2f::size() {
    return sqrt(x * x  + y * y);
}
