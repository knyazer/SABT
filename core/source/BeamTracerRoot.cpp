//
// Created by knyaz on 27/07/22.
//

#include "include/BeamTracerRoot.h"

void BeamTracerRoot::attachToRoot(OctreeRoot *root) {
    stack.push(root);
    this->root = root;
}

void BeamTracerRoot::setCamera(Camera *other) {
    camera = other;
    origin = other->getPosition();
}
