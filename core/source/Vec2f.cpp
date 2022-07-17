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

Vec2f Vec2f::norm() const {
    return operator*(1 / size());
}

Vec2f Vec2f::operator+(Vec2f other) const {
    return {x + other.x, y + other.y};
}

Vec2f Vec2f::operator-(Vec2f other) const {
    return {x - other.x, y - other.y};
}

Vec2f Vec2f::operator*(double other) const {
    return {x * other, y * other};
}

double Vec2f::size() const {
    return sqrt(x * x  + y * y);
}

double Vec2f::dot(Vec2f a, Vec2f b) {
    return a.x * b.x + a.y * b.y;
}

Vec2f Vec2f::tripleProduct(Vec2f a, Vec2f b, Vec2f c) {
    return b * dot(a, c) - a * dot(b, c);
}

Vec2f Vec2f::negate() const {
    return {-x, -y};
}

Vec2f Vec2f::perp() const {
    return {-y, x};
}

double Vec2f::size_1() const {
    return std::abs(x) + std::abs(y);
};

std::ostream& operator<<(std::ostream& os, const Vec2f& val)
{
    os << "[Vec2f](" << val.x << ", " << val.y << ")";
    return os;
}

bool Vec2f::operator==(Vec2f other) const {
    return x == other.x && y == other.y;
}
