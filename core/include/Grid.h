//
// Created by knyaz on 12/08/22.
//

#ifndef SABT_GRID_H
#define SABT_GRID_H

#include "Vec2f.h"
#include "AlignedRect.h"


class Grid {
public:
    AlignedRect base;
    size_t width, height;

    Grid(AlignedRect baseRect, size_t horizontalSize, size_t verticalSize);

    Grid(const Grid &other);

    [[nodiscard]] Vec2f at(size_t xIndex, size_t yIndex) const;

    [[nodiscard]] AlignedRect getCell(size_t xIndex, size_t yIndex) const;
};


#endif //SABT_GRID_H
