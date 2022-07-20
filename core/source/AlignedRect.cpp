//
// Created by knyaz on 6/30/2022.
//

#include "include/AlignedRect.h"

AlignedRect::AlignedRect(Vec2f newMin, Vec2f newMax) {
    min = newMin;
    max = newMax;
}

bool AlignedRect::contains(Vec2f other) const {
    return other.x >= min.x and other.x <= max.x and other.y >= min.y and other.y <= max.y;
}

bool AlignedRect::intersects(const AlignedRect& other) const {
    return !((max.x < other.min.x) ||
             (max.y < other.min.y) ||
             (min.x > other.max.x) ||
             (min.y > other.max.y));
}

Vec2f AlignedRect::getFarthestPointInDirection(Vec2f direction) const { // TODO: make some specific optimizations
    return Polygon({min, {min.x, max.y}, max, {max.x, min.y}}).getFarthestPointInDirection(direction);
}

Vec2f AlignedRect::mid() const {
    return (min + max) * 0.5;
}

AlignedRect::AlignedRect(std::vector<Vec2f> vertices) {
    if (vertices.size() == 0) {
        throw std::runtime_error("Cannot initialize aligned rect with no points");
    }

    min = vertices[0];
    max = vertices[0];

    for (size_t i = 1; i < vertices.size(); i++) {
        if (vertices[i].x < min.x)
            min.x = vertices[i].x;
        if (vertices[i].y < min.y)
            min.y = vertices[i].y;

        if (vertices[i].x > max.x)
            max.x = vertices[i].x;
        if (vertices[i].y > max.y)
            max.y = vertices[i].y;
    }
}
