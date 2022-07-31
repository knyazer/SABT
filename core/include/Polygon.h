//
// Created by knyaz on 6/30/2022.
//

#ifndef SABT_POLYGON_H
#define SABT_POLYGON_H

#include "Vec2f.h"
#include "Shape2d.h"

#include <vector>
#include <cstdint>

/**
 * Polygon in 2 dimensions
 *
 * Represented as a vector of Vec2f-s, which are vertices in an arbitrary order.
 * Able to work with GJK, as overrides getFarthestPointInDirection from Shape2d
 *
 */
class Polygon : public Shape2d {
public:
    std::vector<Vec2f> vertices;

    /// Init with a vector of new vertices
    explicit Polygon(std::vector<Vec2f> newVertices);

    /// Init with a pure C-style array of new vertices
    explicit Polygon(Vec2f *arr, size_t size);

    /// GJK support function
    [[nodiscard]] Vec2f getFarthestPointInDirection(Vec2f direction) const override;
};


#endif //SABT_POLYGON_H
