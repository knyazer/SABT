//
// Created by knyaz on 7/3/2022.
//

#include <SABT.h>

#include <Renderer.h>
#include <GetPath.h>
#include <iostream>
#include <stack>
#include <chrono>

using namespace graphics;

using std::cout, std::endl;

// random stuff
#define WIN_SIZE 1024
#define eps 1e-6

// hyps
#define NUMBER_OF_RAYS_PER_BEAM 0
#define OCTREE_SIZE 9

// params to edit
constexpr size_t RESOLUTION = 256;

// precalc
constexpr size_t NUMBER_OF_BEAMS = NUMBER_OF_RAYS_PER_BEAM == 0 ? RESOLUTION : RESOLUTION / NUMBER_OF_RAYS_PER_BEAM;
constexpr size_t Nh = NUMBER_OF_BEAMS / 2;
constexpr size_t OCTREE_LINEAR_SIZE = 1 << OCTREE_SIZE;
constexpr double MOVE_STEP = double(OCTREE_LINEAR_SIZE) / 100.0;
constexpr double SCALE_FACTOR = WIN_SIZE / RESOLUTION;

int main(int argc, char *args[]) {
    Renderer renderer;

    Camera cam;
    cam.setPosition({OCTREE_LINEAR_SIZE / 5, OCTREE_LINEAR_SIZE / 6, OCTREE_LINEAR_SIZE / 2});
    cam.setRotationByX(Angle::deg(180));
    cam.setRotationByY(Angle::deg(0));
    cam.setRotationByZ(Angle::deg(0));
    cam.setFOV(Angle::deg(80));

    renderer.createWindow("SABT demo", Rect(500, 500, WIN_SIZE, WIN_SIZE));

    Mesh mesh(getPath() + "/models/sponza");
    OctreeRoot world;
    world.fitMesh(mesh, OCTREE_SIZE);
/*
    world.fill({0, 0, 8}, 2, Color::GREEN);
    world.fill({3, 1, 2}, 1, Color::BLUE);
*/
    BeamRenderer beamRenderer(&world, &cam);

    BeamRenderer firstGlanceRenderer(&world, &cam);
    firstGlanceRenderer.update<NUMBER_OF_BEAMS>(NUMBER_OF_RAYS_PER_BEAM);

    // Main render cycle
    while (renderer.update()) {
        renderer.clear(Color::GRAY);

        auto timerBegin = std::chrono::steady_clock::now();

        // Render the scene into colors array
        beamRenderer.fromPrevious<NUMBER_OF_BEAMS>(firstGlanceRenderer);
        auto res = beamRenderer.update<NUMBER_OF_BEAMS>(NUMBER_OF_RAYS_PER_BEAM);

        uint64_t iterTime = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - timerBegin).count();
        std::cout << "iter time:" << iterTime << "us" << std::endl;

        // Show the output of beam renderer on screen
        for (size_t x = 0; x < RESOLUTION; x++) {
            for (size_t y = 0; y < RESOLUTION; y++)
                renderer.fillRect({x * SCALE_FACTOR, y * SCALE_FACTOR, SCALE_FACTOR, SCALE_FACTOR}, res[x][y]);

            delete res[x];
        }
        delete res;

        // position controls
        if (renderer.pressed['j'])
            cam.move({0, 0, -MOVE_STEP});
        if (renderer.pressed['k'])
            cam.move({0, 0, MOVE_STEP});
        if (renderer.pressed['h'])
            cam.move({MOVE_STEP, 0, 0});
        if (renderer.pressed['l'])
            cam.move({-MOVE_STEP, 0, 0});
        if (renderer.pressed['i'])
            cam.move({0, MOVE_STEP, 0});
        if (renderer.pressed['m'])
            cam.move({0, -MOVE_STEP, 0});

        if (renderer.pressed['p'])
            std::cout << cam.getPosition() << std::endl;

        // rotation controls
        auto delta = renderer.getMouseDelta();
        cam.rotateByX(Angle::deg(static_cast<double>(delta.y) / 10));
        cam.rotateByY(Angle::deg(static_cast<double>(delta.x) / 10));
    }

    return 0;
}
