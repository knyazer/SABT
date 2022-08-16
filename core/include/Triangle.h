//
// Created by knyaz on 15/08/22.
//

#ifndef SABT_TRIANGLE_H
#define SABT_TRIANGLE_H

#include "Vec3f.h"
#include "Shape3d.h"
#include "Color.h"

#include <functional>

struct Triangle : public Shape3d {
    Vec3f v1, v2, v3;
    Color c1, c2, c3;

    Triangle(Vec3f firstVertex, Vec3f secondVertex, Vec3f thirdVertex);

    void setColor(Color firstColor, Color secondColor, Color thirdColor);

    void transform(const std::function<Vec3f(const Vec3f&)>& f);

    [[nodiscard]] Vec3f getFarthestPointInDirection(Vec3f direction) const override;

    [[nodiscard]] Vec3f getCenter() const override;
};


#endif //SABT_TRIANGLE_H
