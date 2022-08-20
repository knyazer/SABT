//
// Created by knyaz on 27/07/22.
//

#include "include/BeamTracerRoot.h"

BeamTracerRoot::BeamTracerRoot() {
    rect = AlignedRect({-1, -1}, {1, 1});
}

void BeamTracerRoot::setup(WorldParams *params) {
    stack.push({params->root, params->root->getCube()});
    this->params = params;

    update();
}
