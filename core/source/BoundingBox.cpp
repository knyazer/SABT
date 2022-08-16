//
// Created by knyaz on 16/08/22.
//

#include "include/BoundingBox.h"

BoundingBox::BoundingBox() {
    min = {0, 0, 0};
    max = {0, 0, 0};
}

BoundingBox::BoundingBox(const std::vector<Vec3f>& points) {
    if (points.empty())
        throw std::runtime_error("Why do you provide 0 points in points array? Makes no sense. Exiting.");

    min = points[0];
    max = points[0];

    for (auto point : points) {
        min = Vec3f::min(min, point);
        max = Vec3f::max(max, point);
    }
}

BoundingBox::BoundingBox(const BoundingBox &other) {
    min = other.min;
    max = other.max;
}

void BoundingBox::add(BoundingBox other) {
    if (max.x == 0 && max.y == 0 && max.z == 0 && min.x == 0 && min.y == 0 && min.z == 0) {
        min = other.min;
        max = other.max;
    }
    else {
        min = Vec3f::min(min, other.min);
        max = Vec3f::max(max, other.max);
    }
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
