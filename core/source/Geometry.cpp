//
// Created by knyaz on 6/11/2022.
//

#include "Geometry.h"

//
// !Obsolete!
// Rect
//
/*
Rect::Rect(Vec2f center, double width, double height) {
    setCenter(center);
    setWidth(width);
    setHeight(height);
}

double Rect::getWidth() {
    return width;
}

double Rect::getHeight() {
    return height;
}

Vec2f Rect::getCenter() {
    return center;
}

void Rect::setWidth(double newWidth) {
    if (newWidth <= 0)
        throw std::runtime_error("Width of a rect must be a positive double!");

    this->width = newWidth;
}

void Rect::setHeight(double newHeight) {
    if (newHeight <= 0)
        throw std::runtime_error("Height of a rect must be a positive double!");

    this->height = newHeight;
}

void Rect::setCenter(Vec2f newCenter) {
    this->center = newCenter;
}
*/


//
// !Obsolete!
// Plane
//
/*
Plane::Plane(Vec3f p1, Vec3f p2, Vec3f p3) {
    Vec3f alpha = p2 - p1;
    Vec3f beta = p3 - p1;

    n = Vec3f::cross(alpha, beta);

    d = Vec3f::dot(n, p1);
}

Vec3f Plane::getZeroPoint() {
    return funcs::findIntersection(*this, Line3d({0, 0, 0}, n)).position;
}
*/

//
// Line3d
//

Line3d::Line3d(Vec3f lineOrigin, Vec3f lineDirection) {
    origin = lineOrigin;
    direction = lineDirection;
}

Line2d::Line2d(Vec2f lineOrigin, Vec2f lineDirection) {
    origin = lineOrigin;
    direction = lineDirection;
}
