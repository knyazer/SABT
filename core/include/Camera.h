//
// Created by knyaz on 7/1/2022.
//

#ifndef SABT_CAMERA_H
#define SABT_CAMERA_H

#include "Mat.h"
#include "Angle.h"
#include "Vec3f.h"
#include "Vec2f.h"


/**
 * Core class of drawing engine. The camera controls the way objects are painted on screen.
 * Mainly projects vertices of 3d objects to the screen and transforms rays from the given 2d coordinate
 * to 3d line. Has lazy parameter updating, so the update is pretty fast.
 */
class Camera {
protected:

    /// Some miscellaneous and not so matrices
    Mat<4, 4> projM, posM, xRotM, yRotM, zRotM, perspM, worldToCamera, cameraToWorld;

    /// Defines the camera FOV, horizontal = vertical
    Angle fov;

    /// Needed for lazy updates
    bool needUpdate;

public:

    Camera();

    /// Sets the camera position by given Vec3f
    void setPosition(Vec3f pos);

    /// Gets the camera position in the Vec3f format
    Vec3f getPosition();

    /// Set the absolute rotation of camera around X axis
    void setRotationByX(Angle theta);

    /// Set the absolute rotation of camera around Y axis
    void setRotationByY(Angle theta);

    /// Set the absolute rotation of camera around Z axis
    void setRotationByZ(Angle theta);

    /// Update the projection matrices, and some other corresponding matrices.
    /// Function is kind of slow, so ran lazily. You should run it by yourself only when you have
    /// done some update that is not covered by the lazy updates, so generally never.
    void updateProjectionMatrix();

    /// Sets the FOV of the camera
    void setFOV(Angle angle);

    /// Moves the camera by a given position.
    /// Equivalent to the setPosition(getPosition() + param)
    void move(Vec3f delta);

    /// Projects a point in 3-dimensions onto the 2-dimensional screen
    Vec2f project(Vec3f point);

    /// Returns a direction of a ray passing from the screen point (given as the parameter) in global coords
    Vec3f restore(Vec2f point);

    /// Means that update is required
    void lazyUpdate();

    /// Does the update and sets needUpdate to false
    void doLazyUpdate();

    /// Rotates the camera by a given angle along X axis
    void rotateByX(Angle theta);

    /// Rotates the camera by a given angle along Y axis
    void rotateByY(Angle theta);
};

#endif //SABT_CAMERA_H
