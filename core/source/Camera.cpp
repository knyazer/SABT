//
// Created by knyaz on 7/1/2022.
//

#include "Camera.h"

void Camera::setPosition(Vec3f position) {
    posM = posM.I();

    posM.at(0, 3) = position.x;
    posM.at(1, 3) = position.y;
    posM.at(2, 3) = position.z;
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

void Camera::updateProjectionMatrix() {
    projM = posM * xRotM * yRotM * zRotM;
}
