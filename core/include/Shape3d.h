//
// Created by knyaz on 25/07/22.
//

#ifndef SABT_SHAPE3D_H
#define SABT_SHAPE3D_H

#include "Vec3f.h"

#include <ccd/ccd.h>

class Shape3d {
public:
    [[nodiscard]] virtual Vec3f getFarthestPointInDirection(Vec3f direction) const = 0;

    [[nodiscard]] virtual Vec3f getCenter() const = 0;

    [[nodiscard]] static bool hasIntersection(Shape3d *A, Shape3d *B);
};

namespace Shape3d_ccd {
    void support(const void *obj, const ccd_vec3_t *dir, ccd_vec3_t *vec);

    void center(const void *obj, ccd_vec3_t *center);
}

#endif //SABT_SHAPE3D_H
