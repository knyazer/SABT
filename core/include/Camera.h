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
#include <Vec2f.h>

class Camera {
protected:

    Mat<4, 4> projM, posM, xRotM, yRotM, zRotM, perspM;
    Angle fov;

public:

    Camera();

    void setPosition(Vec3f pos);

    void setRotationByX(Angle theta);

    void setRotationByY(Angle theta);

    void setRotationByZ(Angle theta);

    void updateProjectionMatrix();

    void setFOV(Angle angle);

    Vec2f project(Vec3f point);
};

#endif //SABT_CAMERA_H
