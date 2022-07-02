//
// Created by knyaz on 7/1/2022.
//
// Simple camera representation; clip space transformation included
//

#ifndef SABT_CAMERA_H
#define SABT_CAMERA_H

#include <Mat.h>
#include <Angle.h>
#include <Vec3f.h>

class Camera {
public:
    Mat<4, 4> projM, posM, xRotM, yRotM, zRotM;

    const Angle fov = Angle::deg(90);

    void setPosition(Vec3f pos);

    void setRotationByX(Angle theta);

    void setRotationByY(Angle theta);

    void setRotationByZ(Angle theta);

    void updateProjectionMatrix();
};

#endif //SABT_CAMERA_H
