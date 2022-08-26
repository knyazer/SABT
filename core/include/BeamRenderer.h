//
// Created by knyaz on 25/08/22.
//

#ifndef SABT_BEAM_RENDERER_H
#define SABT_BEAM_RENDERER_H

#include "Camera.h"
#include "OctreeRoot.h"
#include "WorldParams.h"
#include "BeamTracerRoot.h"
#include "BeamTracer.h"
#include "Grid.h"
#include "Color.h"

#include <stack>

class BeamRenderer {
protected:
    WorldParams params;
    BeamTracerRoot beamController;

public:
    BeamRenderer() = default;

    BeamRenderer(OctreeRoot *world, Camera *camera);

    void setup(OctreeRoot *world, Camera *camera);

    void erase();

    template <size_t resolution>
    void fromPrevious(BeamRenderer &other) {
        BeamTracerRoot
    }

    template <size_t resolution>
    Color** update(size_t raysPerBeam=0) {
        const size_t totalResolution = raysPerBeam == 0 ? resolution : (resolution * raysPerBeam);

        auto pixels = new Color*[totalResolution];
        for (size_t i = 0; i < totalResolution; i++)
            pixels[i] = new Color[totalResolution];

        double minBeamWidth = 2.0 / resolution, minBeamHeight = 2.0 / resolution;

        beamController.setup(&params);

        std::stack<BeamTracer *> beams;
        beams.push(&beamController);

        long long fullNodeReturns = 0, smallNodeReturns = 0, perfCounterTotal = 0;
        while (!beams.empty()) {
            BeamTracer *beam = beams.top();
            beams.pop();

            if (beam == nullptr)
                throw std::runtime_error("Nullptr beam in stack encountered. Abort");

            auto res = beam->trace(  0.8 * params.camera->getFOV().rad() / resolution);

            size_t beamX = round((beam->rect.min.x + 1) / minBeamWidth), beamY = round((beam->rect.min.y + 1) / minBeamHeight);

            perfCounterTotal += res.iterations;
            fullNodeReturns += res.fullNodeReturns;
            smallNodeReturns += res.smallNodeReturns;

            // TODO: talk about colors and looking at the backside of cubes
            // TODO: use not quadtree but some sort of dynamic spatial structure, like k-d tree with parameters from previous iteration
            // TODO: choose the optimal beam/ray numbers relation
            // TODO: skip fully filled nodes - return colors immediately (nope, too small of improvement)

            if (res.fill) {
                if (beam->stack.parentEmpty() || beam->rect.width() <= minBeamWidth || beam->rect.height() <= minBeamHeight) {
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
                        if (res.fill)
                            pixels[beamX][beamY] = res.color;
                    }
                } else {
                    beam->makeChildren();

                    for (size_t i = 0; i < 4; i++)
                        beams.push(&beam->children[i]);
                }
            }
        }

        beamController.calculateMinMaxDistances();

        return pixels;
    }
};


#endif //SABT_BEAM_RENDERER_H
