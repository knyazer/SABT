//
// Created by knyaz on 6/30/2022.
//

#ifndef SABT_ALIGNED_RECT_H
#define SABT_ALIGNED_RECT_H

#include "Vec2f.h"
#include "Shape2d.h"
#include "Polygon.h"

#include <vector>

class AlignedRect : public Shape2d {
public:
    Vec2f min, max;

    AlignedRect(Vec2f min, Vec2f max);

    explicit AlignedRect(std::vector<Vec2f> vertices);

    explicit AlignedRect(Vec2f* arr, size_t size);

    [[nodiscard]] bool contains(Vec2f other) const;

    [[nodiscard]] bool intersects(const AlignedRect& other) const;

    [[nodiscard]] Vec2f getFarthestPointInDirection(Vec2f direction) const override;

    [[nodiscard]] Vec2f mid() const;
};


#endif //SABT_ALIGNED_RECT_H
