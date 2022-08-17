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

Color Triangle::getColor(Vec3f point, double side) const {
    double c1 = (Vec3f::cross(v2 - point, v3 - point)).size();
    double c2 = (Vec3f::cross(v3 - point, v1 - point)).size();
    double c3 = (Vec3f::cross(v1 - point, v2 - point)).size();

    double totalArea = c1 + c2 + c3;

    Vec2f coord = (t1 * c1 + t2 * c2 + t3 * c3) * (1.0 / totalArea);

    if (texture == nullptr)
        throw std::runtime_error("Texture for the triangle not set! Committing suicide.");

    int x = round(texture->width() * coord.x),
        y = round(texture->height() * coord.y);

    while (x < 0 || x >= texture->width())
        x -= sign(x) * texture->width();

    while (y < 0 || y >= texture->height())
        y -= sign(y) * texture->height();

    long blurSize = sqrt(texture->height() * texture->width() * side * side / totalArea) / 2;
    long r = 0, g = 0, b = 0;
    long numOfCorrectPixels = 0;

    for (int dx = -blurSize; dx <= blurSize; dx++) {
        for (int dy = -blurSize; dy <= blurSize; dy++) {
            int nX = x + dx, nY = y + dy;
            if (nX < 0 || nX >= texture->width() || nY < 0 || nY >= texture->height())
                continue;

            numOfCorrectPixels += 1;

            r += texture->operator()(nX, nY, 0, 0);
            g += texture->operator()(nX, nY, 0, 1);
            b += texture->operator()(nX, nY, 0, 2);
        }
    }

    if (numOfCorrectPixels == 0)
        throw std::runtime_error("Somehow obtained pixel lies outside of texture. Committing suicide.");

    r /= numOfCorrectPixels;
    g /= numOfCorrectPixels;
    b /= numOfCorrectPixels;

    return {static_cast<ch_t>(r), static_cast<ch_t>(g), static_cast<ch_t>(b)};
}

void Triangle::setTexture(CImg<unsigned char> *texturePtr) {
    texture = texturePtr;
}

void Triangle::setTextureCoordinates(Vec2f first, Vec2f second, Vec2f third) {
    t1 = first;
    t2 = second;
    t3 = third;
}
