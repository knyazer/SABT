//
// Created by knyaz on 15/08/22.
//

#include "include/Triangle.h"

Triangle::Triangle(Vec3f firstVertex, Vec3f secondVertex, Vec3f thirdVertex) {
    v1 = firstVertex;
    v2 = secondVertex;
    v3 = thirdVertex;
}

void Triangle::setColor(Color firstColor, Color secondColor, Color thirdColor) {
    c1 = firstColor;
    c2 = secondColor;
    c3 = thirdColor;
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
