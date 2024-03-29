//
// Created by knyaz on 7/1/2022.
//

#include "include/Camera.h"

Camera::Camera() {
    setFOV(Angle::deg(90));
}

void Camera::setPosition(Vec3f position) {
    posM = posM.I();

    posM.at(0, 3) = -position.x;
    posM.at(1, 3) = -position.y;
    posM.at(2, 3) = -position.z;
    posM.at(3, 3) = 1;

    lazyUpdate();
}

Vec3f Camera::getPosition() {
    return Vec3f(-posM.at(0, 3), -posM.at(1, 3), -posM.at(2, 3));
}

void Camera::setRotationByX(Angle theta) {
    xRotM = xRotM.I();

    xRotM.at(1, 1) = theta.cos();
    xRotM.at(2, 1) = theta.sin();
    xRotM.at(1, 2) = -theta.sin();
    xRotM.at(2, 2) = theta.cos();

    lazyUpdate();
}

void Camera::setRotationByY(Angle theta) {
    yRotM = yRotM.I();

    yRotM.at(0, 0) = theta.cos();
    yRotM.at(0, 2) = theta.sin();
    yRotM.at(2, 0) = -theta.sin();
    yRotM.at(2, 2) = theta.cos();

    lazyUpdate();
}

void Camera::setRotationByZ(Angle theta) {
    zRotM = zRotM.I();

    zRotM.at(0, 0) = theta.cos();
    zRotM.at(1, 0) = theta.sin();
    zRotM.at(0, 1) = -theta.sin();
    zRotM.at(1, 1) = theta.cos();

    lazyUpdate();
}

void Camera::setFOV(Angle angle) {
    fov = angle;

    double zFar = 10e3, zNear = 10e-3;

    perspM.fill(0);

    perspM.at(0, 0) = 1 / (fov / 2).tan();
    perspM.at(1, 1) = 1 / (fov / 2).tan();
    perspM.at(2, 2) = -(zFar + zNear) / (zFar - zNear);
    perspM.at(3, 2) = -1;
    perspM.at(2, 3) = -2 * (zFar * zNear) / (zFar - zNear);
    perspM.at(3, 3) = 0;

    lazyUpdate();
}

void Camera::updateProjectionMatrix() {
    worldToCamera = (xRotM * yRotM * zRotM) * posM;
    worldToCamera.inverse(cameraToWorld);

    projM = perspM * worldToCamera; // First we translate the object, then rotate it abd apply perspective
}

Vec2f Camera::project(Vec3f point) {
    doLazyUpdate();

    /*
     * [ a00 a01 a02 a03 ] * [ x ] = [ x * a00 + y * a01 + z * a02 + a03 ] = [ xt * u ]
     * [ a10 a11 a12 a13 ]   [ y ]   [ ...                               ]   [ yt * u ]
     * [ a20 a21 a22 a23 ]   [ z ]   [ ...                               ]   [ ..     ]
     * [ a30 a31 a32 a33 ]   [ 1 ]   [ ...                               ]   [ u      ]
     *
     *
     *
     */

    double u = point.x * projM.qat(3, 0) + point.y * projM.qat(3, 1) + point.z * projM.qat(3, 2) + projM.qat(3, 3);

    // z clip
    if (std::abs(point.x * projM.qat(2, 0) + point.y * projM.qat(2, 1) + point.z * projM.qat(2, 2) + projM.qat(2, 3)) >= std::abs(u))
        return {42, 42};

    return {
            (point.x * projM.qat(0, 0) + point.y * projM.qat(0, 1) + point.z * projM.qat(0, 2) + projM.qat(0, 3)) / u,
            (point.x * projM.qat(1, 0) + point.y * projM.qat(1, 1) + point.z * projM.qat(1, 2) + projM.qat(1, 3)) / u
    };
}

Vec3f Camera::restore(Vec2f point) {
    doLazyUpdate();

    // scale by fov
    point = point * (fov / 2).tan();

    /*
     * [ z ] = [ a00 a01 a02 a03 ] * [ p.x ] - [ cam.x ]
     * [ y ]   [ a10 a11 a12 a13 ]   [ p.y ]   [ cam.y ]
     * [ x ]   [ a20 a21 a22 a23 ]   [ -1  ]   [ cam.z ]
     * [ . ]   [ a30 a31 a32 a33 ]   [ 1   ]   [ 1     ]
     */

    return {    cameraToWorld.qat(0, 0) * point.x + cameraToWorld.qat(0, 1) * point.y -
                cameraToWorld.qat(0, 2) + cameraToWorld.qat(0, 3) + posM.at(0, 3),

                cameraToWorld.qat(1, 0) * point.x + cameraToWorld.qat(1, 1) * point.y -
                cameraToWorld.qat(1, 2) + cameraToWorld.qat(1, 3) + posM.at(1, 3),

                cameraToWorld.qat(2, 0) * point.x + cameraToWorld.qat(2, 1) * point.y -
                cameraToWorld.qat(2, 2) + cameraToWorld.qat(2, 3) + posM.at(2, 3)
    };

    /*
     * cameraToWorld * projM = I
     * find vector passing through origin and point with coords xs on screen, xw in world
     * projM * xw = xs
     * cameraToWorld * projM * xw = cameraToWorld * xs
     * xw = cameraToWorld * xs
     *
     * projM * [ xw.x ] = [ xs.x ]
     *         [ xw.y ]   [ xs.y ]
     *         [ xw.z ]   [ xs.z ]
     *         [ 1    ]   [ u    ]
     *
     * [ xw.x ] = cameraToWorld * [ xs.x ]
     * [ xw.y ]                   [ xs.y ]
     * [ xw.z ]                   [ xs.z ]
     * [ 1    ]                   [ u    ]
     */
}

void Camera::lazyUpdate() {
    needUpdate = true;
}

void Camera::doLazyUpdate() {
    if (needUpdate) {
        updateProjectionMatrix();
        needUpdate = false;
    }
}

void Camera::move(Vec3f delta) {
    posM.at(0, 3) += delta.x;
    posM.at(1, 3) += delta.y;
    posM.at(2, 3) += delta.z;

    lazyUpdate();
}

void Camera::rotateByX(Angle theta) {
    Mat<4, 4> rot;
    rot = rot.I();

    rot.at(1, 1) = theta.cos();
    rot.at(2, 1) = theta.sin();
    rot.at(1, 2) = -theta.sin();
    rot.at(2, 2) = theta.cos();

    xRotM = xRotM * rot;

    lazyUpdate();
}

void Camera::rotateByY(Angle theta) {
    Mat<4, 4> rot;
    rot = rot.I();

    rot.at(0, 0) = theta.cos();
    rot.at(0, 2) = theta.sin();
    rot.at(2, 0) = -theta.sin();
    rot.at(2, 2) = theta.cos();

    yRotM = yRotM * rot;

    lazyUpdate();
}

Angle Camera::getFOV() const {
    return fov;
}
