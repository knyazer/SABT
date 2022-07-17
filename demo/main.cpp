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
    cam.setFOV(Angle::deg(90));
    cam.updateProjectionMatrix();

    Vec3f point(0, -5, 0.01);
    cout << cam.project(point) << endl;

    renderer.createWindow("SABT", Rect(500, 500, 800, 800));

    for (int i = 0; i < 1000; i++) {
        renderer.clear(GRAY);
        renderer.drawRect(Rect(100 + i, 100, 100, 100), BLUE);
        renderer.update();

        if (renderer.isStopped())
            break;
    }

    return 0;
}