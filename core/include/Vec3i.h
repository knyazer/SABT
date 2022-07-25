//
// Created by knyaz on 6/8/2022.
//

#ifndef SABT_VEC3I_H
#define SABT_VEC3I_H

#include <iostream>

struct Vec3i {
public:
    long long x, y, z;

    Vec3i();

    Vec3i(long long x, long long y, long long z);

    Vec3i operator*(long long factor);

    Vec3i operator+(const Vec3i &other);

    Vec3i operator-(const Vec3i &other);

    friend std::ostream &operator<<(std::ostream &os, const Vec3i &val);
};


#endif //SABT_VEC3I_H
