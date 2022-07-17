//
// Created by knyaz on 7/2/2022.
//

#include "gtest/gtest.h"

#include <Vec3f.h>
#include <Camera.h>

#include <vector>
#include <iostream>

using namespace std;

#define N 10000

TEST(Camera, TranslationTest) {
    Camera cam;

    cam.setPosition({0, 0, 0});
    cam.setRotationByX(Angle::deg(3));
    cam.setRotationByY(Angle::deg(-2));
    cam.setRotationByZ(Angle::deg(5));
    cam.setFOV(Angle::deg(80));
    cam.updateProjectionMatrix();

    double x = static_cast<double>(rand()) / RAND_MAX, y = static_cast<double>(rand()) / RAND_MAX, z = -2;

    vector<Vec2f> results(N);
    Vec2f avg;
    for (size_t i = 0; i < N; i++) {
        double dx = rand() - RAND_MAX / 2, dy = rand() - RAND_MAX / 2, dz = rand() - RAND_MAX / 2;
        cam.setPosition({dx, dy, dz});
        cam.updateProjectionMatrix();

        Vec3f point(x + dx, y + dy, z + dz);
        results[i] = cam.project(point);
        avg = avg + results[i];
    }
    avg = avg * (1.0 / N);

    double error = 0;
    for (size_t i = 0; i < N; i++) {
        error += (avg - results[i]).size();
    }
    error /= N;

    ASSERT_LT(error, 10e-6);
}

TEST(Camera, OutOfBoundsTest) {
    Camera cam;

    cam.setPosition({10, 5, -3});
    cam.setRotationByX(Angle::deg(0));
    cam.setRotationByY(Angle::deg(0));
    cam.setRotationByZ(Angle::deg(0));
    cam.setFOV(Angle::deg(90));
    cam.updateProjectionMatrix();
    for (size_t i = 0; i < N; i++) {
        Vec3f point(rand() - RAND_MAX / 2, rand() - RAND_MAX / 2, rand() - RAND_MAX / 2);
        Vec2f proj = cam.project(point);

        if (point.z > -3)
            EXPECT_TRUE(Vec2f(42, 42) == proj);
    }
}