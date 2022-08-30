//
// Created by knyaz on 25/08/22.
//

#ifndef SABT_BEAM_RENDERER_H
#define SABT_BEAM_RENDERER_H

#define ZERO_ITERATION_BEAM

#include "Camera.h"
#include "OctreeRoot.h"
#include "WorldParams.h"
#include "BeamTracerRoot.h"
#include "BeamTracer.h"
#include "Grid.h"
#include "Color.h"

#include <stack>

template<size_t resolution>
class BeamRenderer {
protected:
    WorldParams params;

    BeamTracerRoot *beamController{nullptr}, *previousBeamController{nullptr};

public:

    size_t **marked{nullptr};

    BeamRenderer() = default;

    BeamRenderer(OctreeRoot *world, Camera *camera) {
        setup(world, camera);
    }

    void setup(OctreeRoot *world, Camera *camera) {
        params = WorldParams(world, camera);

        marked = new size_t *[resolution];
        for (size_t i = 0; i < resolution; i++)
            marked[i] = new size_t[resolution];
    }

    void prepare() {
        std::stack<BeamTracer *> beams;
        beams.push(beamController);

        double minBeamWidth = 2.0 / resolution, minBeamHeight = 2.0 / resolution;

        while (!beams.empty()) {
            BeamTracer *beam = beams.top();
            beams.pop();

            if (beam == nullptr)
                throw std::runtime_error("Nullptr beam in stack encountered. Abort");

            auto res = beam->prepare(previousBeamController, 0.8 * params.camera->getFOV().rad() / resolution);

            size_t beamX = round((beam->rect.min.x + 1) / minBeamWidth), beamY = round(
                    (beam->rect.min.y + 1) / minBeamHeight);

            if (res.fill) {
                marked[beamX][beamY] = beam->marked;
            } else {
                beam->makeChildren();

                for (size_t i = 0; i < 4; i++)
                    beams.push(&beam->children[i]);
            }
        }
    }

    Color **update(size_t raysPerBeam = 0) {
        const size_t totalResolution = raysPerBeam == 0 ? resolution : (resolution * raysPerBeam);

        auto pixels = new Color *[totalResolution];
        for (size_t i = 0; i < totalResolution; i++)
            pixels[i] = new Color[totalResolution];

        double minBeamWidth = 2.0 / resolution, minBeamHeight = 2.0 / resolution;

        for (size_t i = 0; i < resolution; i++)
            for (size_t j = 0; j < resolution; j++)
                marked[i][j] = 0;

#ifdef ZERO_ITERATION_BEAM
        static size_t zeroIterationCounter = 0;
        zeroIterationCounter++;

        if (zeroIterationCounter > 2)
            delete beamController;
        beamController = new BeamTracerRoot();
        beamController->setup(&params, previousBeamController);

        if (zeroIterationCounter == 1)
            previousBeamController = beamController;
#endif

        std::stack<BeamTracer *> beams;
        beams.push(beamController);

        long long fullNodeReturns = 0, smallNodeReturns = 0, perfCounterTotal = 0;
        while (!beams.empty()) {
            BeamTracer *beam = beams.top();
            beams.pop();

            if (beam == nullptr)
                throw std::runtime_error("Nullptr beam in stack encountered. Abort");

            size_t beamX = round((beam->rect.min.x + 1) / minBeamWidth), beamY = round(
                    (beam->rect.min.y + 1) / minBeamHeight);

#ifdef ZERO_ITERATION_BEAM
            TracingResult res;

            if (zeroIterationCounter > 2) {
                res = beam->prepare(previousBeamController, 0);
                //beam->stack.printAll([](auto x){return x.cube; });
                //std::cout << "------" << std::endl;
                if (beamX == 1 && beamY == 1)
                    int a = 1;

                //res = beam->trace(0); // 0.8 * params.camera->getFOV().rad() / resolution);
            }
            else {
                res = beam->trace(0); //0.8 * params.camera->getFOV().rad() / resolution);
                /*if (beamX == 1 && beamY == 1) {
                    beam->stack.printAll([](auto x){return x.cube; });
                    throw std::runtime_error("a");
                };*/
            }
#endif

            perfCounterTotal += res.iterations;
            fullNodeReturns += res.fullNodeReturns;
            smallNodeReturns += res.smallNodeReturns;

            // TODO: talk about colors and looking at the backside of cubes
            // TODO: use not quadtree but some sort of dynamic spatial structure, like k-d tree with parameters from previous iteration
            // TODO: choose the optimal beam/ray numbers relation
            // TODO: skip fully filled nodes - return colors immediately (nope, too small of improvement)
            if (res.fill) {
                if (beam->stack.parentEmpty() || beam->rect.width() <= minBeamWidth ||
                    beam->rect.height() <= minBeamHeight) {
                    if (raysPerBeam != 0) {
                        size_t gridSizeX = raysPerBeam, gridSizeY = raysPerBeam;

                        Grid grid(beam->rect, gridSizeX, gridSizeY);

                        for (size_t x = 0; x < gridSizeX; x++) {
                            for (size_t y = 0; y < gridSizeY; y++) {

                                auto rayRes = beam->castRay(grid.at(x, y));

                                perfCounterTotal += rayRes.iterations;

                                if (rayRes.fill)
                                    pixels[beamX * raysPerBeam + x][beamY * raysPerBeam + y] = rayRes.color;
                            }
                        }
                    } else {
                        pixels[beamX][beamY] = res.color;
                        marked[beamX][beamY] = (beam->marked ? 1 : 0) + ((res.prepareSuccess ? 1 : 0) * 2);
                    }
                } else {
                    beam->makeChildren();

                    for (size_t i = 0; i < 4; i++)
                        beams.push(&beam->children[i]);
                }
            }
        }

        beamController->calculateMinMaxDistances();

        std::cout << "it " << perfCounterTotal << std::endl;

        return pixels;
    }
};


#endif //SABT_BEAM_RENDERER_H
