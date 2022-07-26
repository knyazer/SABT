//
// Created by knyaz on 7/1/2022.
//
// Simple camera representation; clip space transformation included
//

#ifndef SABT_CAMERA_H
#define SABT_CAMERA_H

#include "Mat.h"
#include "Angle.h"
#include "Vec3f.h"
#include "Vec2f.h"

class Camera {
protected:

    Mat<4, 4> projM, posM, xRotM, yRotM, zRotM, perspM, worldToCamera, cameraToWorld;
    Angle fov;

    bool needUpdate;

public:

    Camera();

    void setPosition(Vec3f pos);

    Vec3f getPosition();

    void setRotationByX(Angle theta);

    void setRotationByY(Angle theta);

    void setRotationByZ(Angle theta);

    void updateProjectionMatrix();

    void setFOV(Angle angle);

    void move(Vec3f delta);

    Vec2f project(Vec3f point);

    Vec3f restore(Vec2f point);

    void lazyUpdate();

    void doLazyUpdate();

    void rotateByX(Angle theta);

    void rotateByY(Angle theta);
};

#endif //SABT_CAMERA_H
