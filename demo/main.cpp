//
// Created by knyaz on 7/3/2022.
//

#include <SABT.h>

#include <Renderer.h>
#include <GetPath.h>
#include <iostream>
#include <stack>

using namespace graphics;

using std::cout, std::endl;

// random stuff
#define WIN_SIZE 1024
#define eps 1e-6

// hyps
#define MOVE_STEP 5
#define NUMBER_OF_RAYS_PER_BEAM 4


// params to edit
constexpr size_t RESOLUTION = 128;

// precalc
constexpr double N = RESOLUTION / NUMBER_OF_RAYS_PER_BEAM;
constexpr double Nh = N / 2;

int main(int argc, char *args[]) {
    Renderer renderer;

    Camera cam;
    cam.setPosition({120, 100, 200});
    cam.setRotationByX(Angle::deg(180));
    cam.setRotationByY(Angle::deg(0));
    cam.setRotationByZ(Angle::deg(0));
    cam.setFOV(Angle::deg(80));

    renderer.createWindow("SABT demo", Rect(500, 500, WIN_SIZE, WIN_SIZE));

    Mesh mesh(getPath() + "/models/sponza");
    OctreeRoot world;
    world.fitMesh(mesh, 9);
/*
    world.fill({0, 0, 8}, 2, Color::GREEN);
    world.fill({3, 1, 2}, 1, Color::BLUE);
*/
    auto *params = new WorldParams(&world, &cam);

    // renderer.enableDebugging();
    int debugX = 0, debugY = 0;

    // Main render cycle
    while (renderer.update()) {
        renderer.clear(Color::GRAY);

        ll perfCounterTotal = 0, perfCounterFilled = 0, filledPixels = 0;

        if (renderer.debug) {
            if (renderer.pressed['d'])
                debugX++;
            if (renderer.pressed['a'])
                debugX--;
            if (renderer.pressed['w'])
                debugY--;
            if (renderer.pressed['s'])
                debugY++;

            while (debugX < 0)
                debugX += N;

            while (debugX >= N)
                debugX -= N;

            while (debugY < 0)
                debugY += N;

            while (debugY >= N)
                debugY -= N;
        }

        BeamTracerRoot beamController;
        beamController.setup(params);

        std::stack<BeamTracer *> beams;
        beams.push(&beamController);

        auto tr1 = [](double x) { return round(x * WIN_SIZE / 2 + WIN_SIZE / 2); };
        auto tr2 = [](double x) { return round(x * WIN_SIZE / 2); };
        auto toScreen = [tr1, tr2](const AlignedRect &rect) {
            return Rect(tr1(rect.min.x), tr1(rect.min.y), tr2(rect.width()), tr2(rect.height()));
        };

        while (!beams.empty()) {
            BeamTracer *beam = beams.top();
            beams.pop();

            if (beam == nullptr)
                throw std::runtime_error("Nullptr beam in stack encountered. Abort");

            auto res = beam->trace(0);
            perfCounterTotal += res.iterations;

            // TODO: use not quadtree but some sort of dynamic spatial structure, like k-d tree with parameters from previous iteration
            // TODO: choose the optimal beam/ray numbers relation

            if (res.fill) {
                if (beam->stack.parentEmpty() || beam->rect.width() <= 2.0 /
                                                                       N) { // TODO: skip fully filled nodes - return colors immediately (nope, too small of improvement)
                    int gridSizeX = NUMBER_OF_RAYS_PER_BEAM, gridSizeY = NUMBER_OF_RAYS_PER_BEAM;

                    Grid grid(beam->rect, gridSizeX, gridSizeY);

                    for (size_t x = 0; x < gridSizeX; x++) {
                        for (size_t y = 0; y < gridSizeY; y++) {
                            auto rayRes = beam->castRay(grid.at(x, y));

                            if (rayRes.fill) {
                                renderer.fillRect(toScreen(grid.getCell(x, y)), rayRes.color);
                            }
                        }
                    }
                } else {
                    beam->makeChildren();

                    for (size_t i = 0; i < 4; i++)
                        beams.push(&beam->children[i]);
                }
            }

#ifdef DEBUG
            if (!res.fill) {
                renderer.drawRect(onScreenRect, WHITE);
            }

            if (res.fill) {
                renderer.drawRect(onScreenRect, BLACK);
            }
#endif
        }

        // std::cout << "size: " << N * N << "\titers: " << perfCounterTotal << std::endl;

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
