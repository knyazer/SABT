//
// Created by knyaz on 27/07/22.
//

#ifndef SABT_BEAM_TRACER_ROOT_H
#define SABT_BEAM_TRACER_ROOT_H

#include "BeamTracer.h"
#include "Camera.h"
#include "OctreeRoot.h"

class BeamTracerRoot : public BeamTracer {
public:
    Camera *camera{};

    void attachToRoot(OctreeRoot* root);

    void setCamera(Camera *camera);
};


#endif //SABT_BEAM_TRACER_ROOT_H
