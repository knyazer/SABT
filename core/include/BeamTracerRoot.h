//
// Created by knyaz on 27/07/22.
//

#ifndef SABT_BEAM_TRACER_ROOT_H
#define SABT_BEAM_TRACER_ROOT_H

#include "BeamTracer.h"
#include "Camera.h"
#include "OctreeRoot.h"

/**
 * The root of all beam tracers.
 * Saves the camera, from which it extracts the origin position to set for all other beams, or beam tracers.
 */
class BeamTracerRoot : public BeamTracer {
public:
    Camera *camera{};

    /// Has a specific function, which allows to attach itself directly to octree,
    /// not to the another BeamTracer
    void attachToRoot(OctreeRoot* root);

    /// Just setter for the camera
    void setCamera(Camera *camera);
};


#endif //SABT_BEAM_TRACER_ROOT_H
