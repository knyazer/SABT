//
// Created by knyaz on 06/08/22.
//

#ifndef SABT_WORLD_PARAMS_H
#define SABT_WORLD_PARAMS_H

#include "Camera.h"
#include "OctreeRoot.h"

struct WorldParams {
    OctreeRoot *root{};
    Camera *camera{};

    WorldParams(OctreeRoot *newRoot, Camera *newCamera) {
        root = newRoot;
        camera = newCamera;
    }
};


#endif //SABT_WORLD_PARAMS_H
