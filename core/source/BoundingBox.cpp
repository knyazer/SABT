//
// Created by knyaz on 16/08/22.
//

#include "include/BoundingBox.h"

BoundingBox::BoundingBox() {
    min = {0, 0, 0};
    max = {1, 1, 1};
}

BoundingBox::BoundingBox(const std::vector<Vec3f>& points) {
    if (points.empty())
        throw std::runtime_error("Why do you provide 0 points in points array? Makes no sense. Exiting.");

    min = points[0];
    max = points[0];

    for (auto point : points) {
        min = {min2(point.x, min.x), min2(point.y, min.y), min2(point.z, min.z)};
        max = {max2(point.x, max.x), max2(point.y, max.y), max2(point.z, max.z)};
    }
}

BoundingBox::BoundingBox(const BoundingBox &other) {
    min = other.min;
    max = other.max;
}

void BoundingBox::add(BoundingBox other) {
    min = {min2(other.min.x, min.x), min2(other.min.y, min.y), min2(other.min.z, min.z)};
    max = {max2(other.max.x, max.x), min2(other.max.y, max.y), min2(other.max.z, max.z)};
}

double BoundingBox::width() const {
    return max.x - min.x;
}

double BoundingBox::height() const {
    return max.y - min.y;
}

double BoundingBox::depth() const {
    return max.z - min.z;
}
