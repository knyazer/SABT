//
// Created by knyaz on 27/07/22.
//

#ifndef SABT_BEAM_TRACER_ROOT_H
#define SABT_BEAM_TRACER_ROOT_H

#include "BeamTracer.h"
#include "Camera.h"
#include "OctreeRoot.h"
#include "WorldParams.h"

#include <stack>

/**
 * The root of all beam tracers.
 * Saves the camera, from which it extracts the origin position to set for all other beams, or beam tracers.
 */
class BeamTracerRoot : public BeamTracer {
public:
    BeamTracerRoot();

    /// Has a specific function, which allows to attach itself directly to octree,
    /// not to another BeamTracer
    void setup(WorldParams *params, BeamTracer *previousBeamRoot = nullptr);
};


#endif //SABT_BEAM_TRACER_ROOT_H
