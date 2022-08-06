//
// Created by knyaz on 6/30/2022.
//

#ifndef SABT_ALIGNED_RECT_H
#define SABT_ALIGNED_RECT_H

#include "Vec2f.h"
#include "Shape2d.h"
#include "Polygon.h"
#include "Biplet.h"

#include <vector>

/**
 * Axis Aligned Bounding Box, or, simply, Aligned Rect.
 * Can be defined as a bounding box for a given set of points, or
 * explicitly using min-max vectors.
 */
class AlignedRect : public Shape2d {
public:
    /// These vectors contain coordinates of the minx,miny maxx,maxy corners of the rect.
    Vec2f min, max;

    AlignedRect() = default;

    AlignedRect(Vec2f A, Vec2f B);

    /// Initialize as a bounding box for the set of points
    AlignedRect(std::vector<Vec2f> vertices);

    /// Initialize as a bounding box for the set of points, but with C-style array as parameter
    AlignedRect(Vec2f* arr, size_t size);

    /// Check whether a point lies inside the rect
    [[nodiscard]] bool contains(Vec2f other) const;

    /// Check whether the current rect intersects with other (given) rect
    [[nodiscard]] bool intersects(const AlignedRect& other) const;

    /// Get the farthest point in a particular direction - the same as in Polygon, TODO: make faster
    [[nodiscard]] Vec2f getFarthestPointInDirection(Vec2f direction) const override;

    /// Returns the center point of the AABB
    [[nodiscard]] Vec2f mid() const;

    /// Returns all the 4 vertices in an arbitrary order
    [[nodiscard]] Vec2f* getVertices() const;

    [[nodiscard]] double width() const;

    [[nodiscard]] double height() const;

    /// Access the vertices using Biplet
    Vec2f operator[](Biplet index);
};


#endif //SABT_ALIGNED_RECT_H
