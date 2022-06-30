//
// Created by knyaz on 6/12/2022.
//

#include "Cube.h"

Cube::Cube() {
    center = Vec3f(0, 0, 0);
    size = 0;
}

Cube::Cube(Vec3f cubeCenter, double cubeSize) {
    center = cubeCenter;
    size = cubeSize;
}


