//
// Created by knyaz on 16/08/22.
//

#ifndef SABT_BOUNDING_BOX_H
#define SABT_BOUNDING_BOX_H

#include "Vec3f.h"
#include "BasicMath.h"

#include <vector>

/**
 * Represents a 3 dimensional bounding box. Can be constructed by a list of points (Vec3f),
 * can be merged and have a bit of other functionality.
 */
struct BoundingBox {
    Vec3f min, max;

    BoundingBox();

    explicit BoundingBox(const std::vector<Vec3f>& points);

    BoundingBox(const BoundingBox& other);

    void add(BoundingBox other);

    [[nodiscard]] double width() const;
    [[nodiscard]] double height() const;
    [[nodiscard]] double depth() const;

};


#endif //SABT_BOUNDING_BOX_H
