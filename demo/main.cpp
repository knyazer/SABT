//
// Created by knyaz on 7/3/2022.
//

#include <Renderer.h>
#include <SABT.h>
#include <iostream>
#include <stack>

using namespace graphics;

using std::cout, std::endl;

#define WIN_SIZE 960
#define eps 1e-6
constexpr double N = 256;
constexpr double Nh = N / 2;

int main(int argc, char *args[]) {
    Renderer renderer;

    Camera cam;
    cam.setPosition({0, 0, 0});
    cam.setRotationByX(Angle::deg(180));
    cam.setRotationByY(Angle::deg(0));
    cam.setRotationByZ(Angle::deg(0));
    cam.setFOV(Angle::deg(80));

    renderer.createWindow("SABT demo", Rect(500, 500, WIN_SIZE, WIN_SIZE));

    OctreeRoot world;
    for (int i = 0; i < 4; i++)
        world.grow();

    world.fill({0, 0, 8}, 2, GREEN);
    world.fill({3, 1, 2}, 1, BLUE);

    WorldParams *params = new WorldParams(&world, &cam);

    // renderer.enableDebugging();
    int debugX = 0, debugY = 0;

    // Main render cycle
    while (renderer.update()) {
        renderer.clear(GRAY);

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

        auto tr1 = [](double x) {return round(x * WIN_SIZE / 2 + WIN_SIZE / 2);};
        auto tr2 = [](double x) {return round(x * WIN_SIZE / 2);};

        while (!beams.empty()) {
            BeamTracer* beam = beams.top();
            beams.pop();

            if (beam == nullptr)
                throw std::runtime_error("Nullptr beam in stack encountered. Abort");

            auto res = beam->trace(0);
            perfCounterTotal += res.iterations;

            if (res.fill) {
                if (beam->stack.parentEmpty() || beam->rect.width() <= 2.0 / N)
                    renderer.drawRect(Rect(tr1(beam->rect.mid().x), tr1(beam->rect.mid().y), WIN_SIZE / N, WIN_SIZE / N), res.color);
                else {
                    beam->makeChildren();

                    for (size_t i = 0; i < 4; i++)
                        beams.push(&beam->children[i]);
                }
            }
        }


        /*

        Vec2f* beamVertices = new Vec2f[4];
        Vec3f restored[4];

        for (int xi = 0; xi < N; xi++) {
            for (int yi = 0; yi < N; yi++) {
                AlignedRect rect({static_cast<double>(xi - Nh) / Nh, static_cast<double>(yi - Nh) / Nh},
                                 {static_cast<double>((xi + 1) - Nh) / Nh, static_cast<double>((yi + 1) - Nh) / Nh });


                beamVertices = rect.getVertices();
                for (size_t j = 0; j < 4; j++)
                    restored[j] = cam.restore(beamVertices[j]);

                BeamTracer tracer;

                if (renderer.debug && debugX == xi && debugY == yi)
                    tracer.verbose = true;

                tracer.set(cam.getPosition(), restored);

                tracer.attach(beamRoot);

                auto res = tracer.trace(0);

                perfCounterTotal += res.iterations;

                // Showing the beam

                Vec2f mid = rect.min;
                Vec2f dim = rect.max - rect.min;

                if (res.fill)
                    renderer.drawRect(Rect(tr(mid.x), tr(mid.y), WIN_SIZE / N, WIN_SIZE / N), res.color);

                if (renderer.debug && debugX == xi && debugY == yi)
                    renderer.drawRect(Rect(tr(mid.x + dim.x / 2) - 2.0, tr(mid.y + dim.y / 2) - 2.0, 4.0, 4.0), WHITE);
            }
        }
        */

        std::cout << perfCounterTotal << std::endl;

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
