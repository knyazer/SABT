//
// Created by knyaz on 25/07/22.
//

#include "include/Shape3d.h"

bool Shape3d::hasIntersection(Shape3d *A, Shape3d *B) {
    ccd_t ccd;
    CCD_INIT(&ccd);

    ccd.support1 = Shape3d_ccd::support;
    ccd.support2 = Shape3d_ccd::support;

    ccd.center1 = Shape3d_ccd::center;
    ccd.center2 = Shape3d_ccd::center;

    ccd.mpr_tolerance = 0.0001;

    return ccdMPRIntersect(A, B, &ccd);
}

void Shape3d_ccd::support(const void *obj, const ccd_vec3_t *dir, ccd_vec3_t *vec) {
    Vec3f res = ((Shape3d*)(obj))->getFarthestPointInDirection({dir->v[0], dir->v[1], dir->v[2]});

    vec->v[0] = res.x;
    vec->v[1] = res.y;
    vec->v[2] = res.z;
}

void Shape3d_ccd::center(const void *obj, ccd_vec3_t *center) {
    Vec3f res = ((Shape3d*)(obj))->getCenter();

    center->v[0] = res.x;
    center->v[1] = res.y;
    center->v[2] = res.z;
}