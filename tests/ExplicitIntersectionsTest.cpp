//
// Created by knyaz on 31/08/22.
//

#include "gtest/gtest.h"

#include <SABT.h>

using namespace ExplicitIntersections;

#define N 500000

TEST(ExplicitIntersectionTest, BeamCubeIntersectionTest) {
    long long incorrect = 0, output01 = 0, output10 = 0, output00 = 0, output11 = 0;
    Camera cam;

    for (size_t i = 0; i < N; i++) {
        if (i % 100 == 0) {
            cam.setPosition({
                                    static_cast<double>((rand() % 100000)) / 100000.0 * 5 - 2,
                                    static_cast<double>((rand() % 100000)) / 100000.0 * 5 - 2,
                                    static_cast<double>((rand() % 100000)) / 100000.0 * 5 - 2
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
        bool MPRResult = Shape3d::hasIntersection(&cube, &beam);

        bool explicitResult = BeamCubeIntersection(&beam, &cube);
        // comparison
        if (explicitResult != MPRResult) {
            if (explicitResult == 0)
                output01++;
            else
                output10++;


            bool _ = BeamCubeIntersection(&beam, &cube);

            incorrect++;
        }
        else {
            if (explicitResult == 0)
                output00++;
            else
                output11++;
        }
    }

    std::cout << "Beam failures: " << incorrect << std::endl;
    std::cout << "old on top, new on right \n "
                 "      no \t\t yes\n"
                 "no  " << output00 << "\t\t" << output01 << "\n" <<
                 "yes " << output10 << "\t\t" << output11 << "\n";
    EXPECT_LE(incorrect, N / 1000);
}