//
// Created by knyaz on 6/12/2022.
//

#ifndef SABT_CUBE_H
#define SABT_CUBE_H

#include "Vec3f.h"

struct Cube {
    Vec3f center;
    double size;

    Cube();
    Cube(Vec3f cubeCenter, double cubeSize);
};


#endif //SABT_CUBE_H
