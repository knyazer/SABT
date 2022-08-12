//
// Created by knyaz on 12/08/22.
//

#include "include/Grid.h"

Grid::Grid(AlignedRect baseRect, size_t horizontalSize, size_t verticalSize) {
    base = baseRect;
    width = horizontalSize;
    height = verticalSize;
}

Grid::Grid(const Grid &other) {
    base = other.base;
    width = other.width;
    height = other.height;
}

Vec2f Grid::at(size_t xIndex, size_t yIndex) const {
    return base.min + Vec2f((xIndex + 0.5) * double(base.width() / width), (yIndex + 0.5) * double(base.height() / height));
}

AlignedRect Grid::getCell(size_t xIndex, size_t yIndex) const {
    double cellWidth = base.width() / width, cellHeight = base.height() / height;

    return AlignedRect(
            base.min + Vec2f(xIndex * cellWidth, yIndex * cellHeight),
            base.min + Vec2f((xIndex + 1) * cellWidth, (yIndex + 1) * cellHeight)
            );
}
