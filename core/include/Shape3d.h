//
// Created by knyaz on 25/07/22.
//

#ifndef SABT_SHAPE3D_H
#define SABT_SHAPE3D_H

#include "Vec3f.h"

#include <ccd/ccd.h>

/**
 * Base class for all 3-dimensional convex shapes.
 * Used for runtime polymorphism. Consists of a few pure virtual function to allow libccd MPR to work.
 */
class Shape3d {
public:
    [[nodiscard]] virtual Vec3f getFarthestPointInDirection(Vec3f direction) const = 0;

    /**
     * Should return some sort of a center of a shape, though it is allowed to
     * return just an arbitrary point inside the shape.
     */
    [[nodiscard]] virtual Vec3f getCenter() const = 0;

    /**
     * Finds the intersection between two given pointers to Shape3d using the libccd MPR.
     */
    [[nodiscard]] static bool hasIntersection(Shape3d *A, Shape3d *B);
};

/**
 * Namespace for better integration of the libccd functions into the project.
 */
namespace Shape3d_ccd {
    void support(const void *obj, const ccd_vec3_t *dir, ccd_vec3_t *vec);

    void center(const void *obj, ccd_vec3_t *center);
}

#endif //SABT_SHAPE3D_H
