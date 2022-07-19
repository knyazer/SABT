//
// Created by knyaz on 6/12/2022.
//

#ifndef SABT_CUBE_H
#define SABT_CUBE_H

#include <Vec3i.h>
#include <vector>

using ll = long long;

struct Cube {
    Vec3i pos;
    ll size;

    Cube();
    Cube(Vec3i pos, ll size);
    std::vector<Vec3i> getVertices();
};


#endif //SABT_CUBE_H
