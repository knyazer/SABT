//
// Created by knyaz on 26/07/22.
//

#include "gtest/gtest.h"

#include <SABT.h>

#define N 50000

TEST(Beam, CameraBeamEqualityOfCubeTest) {
    long long incorrect = 0;
    Camera cam;

    for (size_t i = 0; i < N; i++) {
        if (i % 100 == 0) {
            cam.setPosition({
                                    static_cast<double>(rand() % 5 - 2),
                                    static_cast<double>(rand() % 5 - 2),
                                    static_cast<double>(rand() % 5 - 2)
                            });

            cam.setRotationByX(Angle::deg(rand() % 20 - 10));
            cam.setRotationByY(Angle::deg(rand() % 20 - 10));
            cam.setRotationByZ(Angle::deg(rand() % 20 - 10));
            cam.setFOV(Angle::deg(70 + rand() % 40 - 20));
        }

        Cube cube({rand() % 10 - 5, rand() % 10 - 5, rand() % 10 - 5}, rand() % 5);

        Vec2f mn(static_cast<double>(rand() % 30) / 10 - 1.5, static_cast<double>(rand() % 20) / 10 - 1.5);
        AlignedRect beamRect(mn,
                             {mn.x + static_cast<double>(rand() % 10) / 20,
                              mn.y + static_cast<double>(rand() % 10) / 20});


        // Beam construction part
        auto beamVertices = beamRect.getVertices();
        Vec3f restored[4];
        for (size_t j = 0; j < 4; j++)
            restored[j] = cam.restore(beamVertices[j]);

        Beam beam(cam.getPosition(), restored);
        bool newResult = Shape3d::hasIntersection(&cube, &beam);

        // GJK part
        Vec2f projected[8];
        size_t pointsNumber = 0;
        Vec3i *cubeVertices = cube.getVertices();
        for (size_t j = 0; j < 8; j++) {
            Vec3i &vertex = cubeVertices[j];
            Vec2f proj = cam.project(Vec3f(vertex.x, vertex.y, vertex.z)); // TODO: add none project, not 42 42
            if (proj.x != 42 || proj.y != 42)
                projected[pointsNumber++] = proj;
        }

        bool oldResult = pointsNumber != 0 && GJK::GJK(beamRect, Polygon(projected, pointsNumber));

        // comparison
        if (oldResult != newResult)
            incorrect++;
    }

    std::cout << "Beam failures: " << incorrect << std::endl;
    EXPECT_LE(incorrect, N / 3);
}