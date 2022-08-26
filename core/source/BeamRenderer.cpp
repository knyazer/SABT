//
// Created by knyaz on 25/08/22.
//

#include "include/BeamRenderer.h"

BeamRenderer::BeamRenderer(OctreeRoot *world, Camera *camera) {
    setup(world, camera);
}

void BeamRenderer::setup(OctreeRoot *world, Camera *camera) {
    params = WorldParams(world, camera);
}

void BeamRenderer::erase() {
    beamController.~BeamTracerRoot();
    beamController = BeamTracerRoot();
}
