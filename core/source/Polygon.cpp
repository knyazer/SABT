//
// Created by knyaz on 6/30/2022.
//

#include "include/Polygon.h"

Vec2f Polygon::getFarthestPointInDirection(Vec2f direction) const {
    double bestVertexProj = Vec2f::dot(vertices[0], direction);
    size_t bestVertexI = 0, size = vertices.size();

    for (size_t i = 1; i < size; i++) {
        double proj = Vec2f::dot(vertices[i], direction);
        if (proj > bestVertexProj) {
            bestVertexProj = proj;
            bestVertexI = i;
        }
    }

    return vertices[bestVertexI];
}

Polygon::Polygon(std::vector<Vec2f> newVertices) {
    vertices = std::move(newVertices);
}

Polygon::Polygon(Vec2f *arr, size_t size) {
    vertices.assign(size, Vec2f());

    for (size_t i = 0; i < size; i++)
        vertices[i] = arr[i];
}
