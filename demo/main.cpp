//
// Created by knyaz on 7/3/2022.
//

#include <Renderer.h>
#include <SABT.h>
#include <iostream>

using namespace graphics;

using std::cout, std::endl;

int main(int argc, char* args[]) {
    Renderer renderer;

    Camera cam;
    cam.setPosition({0, 0, 0});
    cam.setRotationByX(Angle::deg(0));
    cam.setRotationByY(Angle::deg(0));
    cam.setRotationByZ(Angle::deg(0));
    cam.setFOV(Angle::deg(80));

    std::vector<Vec3f> pointCloud;

    for (size_t i = 0; i < 3000; i++) {
        Vec3f point(double(2 * rand()) / RAND_MAX,
                    double(2 * rand()) / RAND_MAX,
                    double(2 * rand()) / RAND_MAX);

        if (point.size() <= 1) {
            pointCloud.push_back(point);
        }
    }

    renderer.createWindow("SABT", Rect(500, 500, 800, 800));

    // Main render cycle
    while (renderer.update()) {
        renderer.clear(GRAY);

        for (auto point : pointCloud) {
            auto pos = cam.project(point);

            if (std::abs(pos.x) < 1 && std::abs(pos.y) < 1) {
                pos.x = (pos.x + 1) * 400;
                pos.y = (pos.y + 1) * 400;

                renderer.drawRect(Rect(pos.x - 2, pos.y - 2, 5.0, 5.0), BLACK);
            }
        }

        // position controls
        if (renderer.pressed['j'])
            cam.move({0, 0, -0.1});
        if (renderer.pressed['k'])
            cam.move({0, 0, 0.1});
        if (renderer.pressed['h'])
            cam.move({0.1, 0, 0});
        if (renderer.pressed['l'])
            cam.move({-0.1, 0, 0});
        if (renderer.pressed['i'])
            cam.move({0, 0.1, 0});
        if (renderer.pressed['m'])
            cam.move({0, -0.1, 0});

        // rotation controls
        auto delta = renderer.getMouseDelta();
        cam.rotateByX(Angle::deg(static_cast<double>(delta.y) / 10));
        cam.rotateByY(Angle::deg(static_cast<double>(delta.x) / 10));
    }

    return 0;
}