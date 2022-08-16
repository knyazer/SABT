//
// Created by knyaz on 15/08/22.
//

#include "include/Triangle.h"

Triangle::Triangle(Vec3f firstVertex, Vec3f secondVertex, Vec3f thirdVertex) {
    v1 = firstVertex;
    v2 = secondVertex;
    v3 = thirdVertex;
}

Vec3f Triangle::getCenter() const {
    return v1;
}

Vec3f Triangle::getFarthestPointInDirection(Vec3f direction) const {
    double d1 = Vec3f::dot(v1, direction),
           d2 = Vec3f::dot(v2, direction),
           d3 = Vec3f::dot(v3, direction);

    if (d1 > d2) {
        if (d1 > d3)
            return v1;
        else
            return v3;
    }
    else {
        if (d2 > d3)
            return v2;
        else
            return v3;
    }
}

void Triangle::transform(const std::function<Vec3f(const Vec3f &)>& f) {
    v1 = f(v1);
    v2 = f(v2);
    v3 = f(v3);
}

Color Triangle::getColor(Vec3f point) const {
    double c1 = (Vec3f::cross(v2 - point, v3 - point)).size();
    double c2 = (Vec3f::cross(v3 - point, v1 - point)).size();
    double c3 = (Vec3f::cross(v1 - point, v2 - point)).size();

    double sum = c1 + c2 + c3;
    c1 /= sum;
    c2 /= sum;
    c3 /= sum;

    Vec2f coord = t1 * c1 + t2 * c2 + t3 * c3;

    if (texture == nullptr)
        throw std::runtime_error("Texture for the triangle not set! Committing suicide.");

    int x = round(texture->width() * coord.x),
        y = round(texture->height() * coord.y);

    while (x < 0 || x >= texture->width())
        x -= sign(x) * texture->width();

    while (y < 0 || y >= texture->height())
        y -= sign(y) * texture->height();

    return {texture->operator()(x, y, 0, 0), texture->operator()(x, y, 0, 1), texture->operator()(x, y, 0, 2)};
}

void Triangle::setTexture(CImg<unsigned char> *texturePtr) {
    texture = texturePtr;
}

void Triangle::setTextureCoordinates(Vec2f first, Vec2f second, Vec2f third) {
    t1 = first;
    t2 = second;
    t3 = third;
}
