//
// Created by knyaz on 27/08/22.
//

#include "include/Polytope.h"

Polytope::Polytope(std::vector<Vec3f> _vertices) : vertices(_vertices) {
    center = Vec3f(0, 0, 0);
    for (auto &v : vertices)
        center = center + v;

    center = center * (1.0 / vertices.size());
}

Vec3f Polytope::getFarthestPointInDirection(Vec3f direction) const {
    if (vertices.size() == 0)
        throw std::runtime_error("Not allowed to use polytopes with zero vertices. Probably forgot to initialize it. Committing suicide.");

    double maxProd = Vec3f::dot(vertices[0], direction);
    size_t maxIndex = 0;

    for (size_t i = 1; i < vertices.size(); i++) {
        double prod = Vec3f::dot(vertices[i], direction);

        if (prod > maxProd) {
            maxProd = prod;
            maxIndex = i;
        }
    }

    return vertices[maxIndex];
}

Vec3f Polytope::getCenter() const {
   return center;
}
