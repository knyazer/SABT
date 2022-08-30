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
    Vec3f normal = Vec3f::cross(v2 - v1, v3 - v1);

    long r = 0, g = 0, b = 0;

    if (normal.x == 0 && normal.y == 0 && normal.z == 0) {
        for (int x = 0; x < texture->width(); x++) {
            for (int y = 0; y < texture->height(); y++) {
                r += texture->operator()(x, y, 0, 0);
                g += texture->operator()(x, y, 0, 1);
                b += texture->operator()(x, y, 0, 2);
            }
        }

        r /= texture->width() * texture->height();
        g /= texture->width() * texture->height();
        b /= texture->width() * texture->height();

        return {static_cast<ch_t>(r), static_cast<ch_t>(g), static_cast<ch_t>(b)};
    }

    Vec3f relative = point - v1;

    double distToPlane = Vec3f::dot(relative, normal);
    Vec3f projPoint = point;

    double c1 = (Vec3f::cross(v2 - projPoint, v3 - projPoint)).size();
    double c2 = (Vec3f::cross(v3 - projPoint, v1 - projPoint)).size();
    double c3 = (Vec3f::cross(v1 - projPoint, v2 - projPoint)).size();

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

    long blurSize = 0;//sqrt(texture->height() * texture->width() * side * side / totalArea) / 3 + 1;
    long numOfCorrectPixels = 0;
    bool whitePixel = false;

    for (int dx = -blurSize; dx <= blurSize; dx++) {
        for (int dy = -blurSize; dy <= blurSize; dy++) {
            int nX = x + dx, nY = y + dy;
            if (nX < 0 || nX >= texture->width() || nY < 0 || nY >= texture->height())
                continue;

            // Skip white pixels cuz it is usually the background
            ch_t cr = texture->operator()(nX, nY, 0, 0),
                 cg = texture->operator()(nX, nY, 0, 1),
                 cb = texture->operator()(nX, nY, 0, 2);

            if (cr == 255 && cg == 255 && cb == 255) {
                whitePixel = true;
                continue;
            }

            numOfCorrectPixels += 1;

            r += cr;
            g += cg;
            b += cb;
        }
    }

    if (numOfCorrectPixels == 0 && whitePixel)
        return {255, 255, 255};

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
