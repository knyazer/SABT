//
// Created by knyaz on 6/30/2022.
//

#ifndef SABT_POLYGON_H
#define SABT_POLYGON_H

#include <Vec2f.h>
#include <Shape2d.h>

#include <vector>

class Polygon : public Shape2d {
public:
    std::vector<Vec2f> vertices;

    explicit Polygon(std::vector<Vec2f> newVertices);

    [[nodiscard]] Vec2f getFarthestPointInDirection(Vec2f direction) const override;
};


#endif //SABT_POLYGON_H
