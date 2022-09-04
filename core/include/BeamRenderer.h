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

    BeamRenderer(OctreeRoot *world, Camera camera) {
        setup(world, camera);
    }

    void setup(OctreeRoot *world, Camera camera) {
        params = WorldParams(world, camera);

        marked = new size_t *[resolution];
        for (size_t i = 0; i < resolution; i++)
            marked[i] = new size_t[resolution];
    }

    void updateCamera(Camera camera) {
        params.camera = camera;
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
        beamController->setup(params, previousBeamController);

        if (zeroIterationCounter == 1)
            previousBeamController = beamController;
#endif
    double DQDFactor = 0.8 * params.camera.getFOV().rad() / resolution;
    std::stack<BeamTracer *> beams;
#if 1
        if (zeroIterationCounter > 2) {
            beams.push(beamController);

            // double minBeamWidth = 2.0 / resolution, minBeamHeight = 2.0 / resolution;

            while (!beams.empty()) {
                BeamTracer *beam = beams.top();
                beams.pop();

                if (beam == nullptr)
                    throw std::runtime_error("Nullptr beam in stack encountered. Abort");

                auto res = beam->prepare(previousBeamController, DQDFactor, beam->rect.width() <= minBeamWidth);

                size_t beamX = round((beam->rect.min.x + 1) / minBeamWidth), beamY = round(
                        (beam->rect.min.y + 1) / minBeamHeight);

                if (res.fill) {
                    if (beam->rect.width() <= minBeamWidth) {
                        pixels[beamX][beamY] = res.color;
                        beam->precomputed = true;
                        marked[beamX][beamY] = 1;
                    } else {
                        if (beam->children == nullptr)
                            beam->makeChildren();

                        for (size_t i = 0; i < 4; i++)
                            beams.push(&beam->children[i]);
                    }
                }
            }
        }

        beamController->updatePrecomputed();
#endif

        // std::stack<BeamTracer *> beams;
        beams.push(beamController);
        size_t skipped = 0, traced = 0;
        size_t skippedArea = 0, tracedArea = 0;


        long long fullNodeReturns = 0, smallNodeReturns = 0, perfCounterTotal = 0;
        while (!beams.empty()) {
            BeamTracer *beam = beams.top();
            beams.pop();

            if (beam == nullptr)
                throw std::runtime_error("Nullptr beam in stack encountered. Abort");

            size_t beamX = round((beam->rect.min.x + 1) / minBeamWidth), beamY = round(
                    (beam->rect.min.y + 1) / minBeamHeight);
            size_t beamW = round(beam->rect.width() / minBeamWidth), beamH = round(beam->rect.height() / minBeamHeight);

#ifdef ZERO_ITERATION_BEAM
            TracingResult res;

            if (zeroIterationCounter > 2) {
                if (beam->precomputed) {
                    skipped++;
                    skippedArea += beamW * beamH;
                    continue;
                }
                else {
                    res = beam->trace(DQDFactor);
                    traced++;

                    if (beamW * beamH == 1)
                        tracedArea += beamW * beamH;
                }
            }
            else {
                res = beam->trace(DQDFactor); //0.8 * params.camera->getFOV().rad() / resolution);
            }
#endif

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
                    }
                } else {
                    if (beam->children == nullptr)
                        beam->makeChildren();

                    for (size_t i = 0; i < 4; i++) {
                        beam->children[i].attachStack(beam);
                        beams.push(&beam->children[i]);
                    }
                }
            }
        }

        beamController->calculateMinMaxDistances();

        std::cout << "it " << perfCounterTotal << " sk " << skipped << " (" << skippedArea << ")\t" << " tr " << traced <<
                    " (" << tracedArea << ")" << std::endl;

        return pixels;
    }
};


#endif //SABT_BEAM_RENDERER_H
