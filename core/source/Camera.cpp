//
// Created by knyaz on 7/1/2022.
//

#include "Camera.h"

Camera::Camera() {
    setFOV(Angle::deg(90));
}

void Camera::setPosition(Vec3f position) {
    posM = posM.I();

    posM.at(0, 3) = -position.x;
    posM.at(1, 3) = -position.y;
    posM.at(2, 3) = -position.z;
}

void Camera::setRotationByX(Angle theta) {
    xRotM = xRotM.I();

    xRotM.at(1, 1) = theta.cos();
    xRotM.at(2, 1) = theta.sin();
    xRotM.at(1, 2) = -theta.sin();
    xRotM.at(2, 2) = theta.cos();
}

void Camera::setRotationByY(Angle theta) {
    yRotM = yRotM.I();

    yRotM.at(0, 0) = theta.cos();
    yRotM.at(0, 2) = theta.sin();
    yRotM.at(2, 0) = -theta.sin();
    yRotM.at(2, 2) = theta.cos();
}

void Camera::setRotationByZ(Angle theta) {
    zRotM = zRotM.I();

    zRotM.at(0, 0) = theta.cos();
    zRotM.at(1, 0) = theta.sin();
    zRotM.at(0, 1) = -theta.sin();
    zRotM.at(1, 1) = theta.cos();
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
}

void Camera::updateProjectionMatrix() {
    projM = perspM * (xRotM * yRotM * zRotM) * posM; // First we translate the object, then rotate it abd apply perspective
}

Vec2f Camera::project(Vec3f point) {
    Mat<4, 1> pointV({{point.x}, {point.y}, {point.z}, {1}});

    auto res = projM * pointV;

    for (size_t i = 0; i < 3; i++)
        res.at(i, 0) /= res.at(3, 0);

    return {res.at(0, 0), res.at(1, 0)};
}
