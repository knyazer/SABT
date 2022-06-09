//
// Created by knyaz on 6/8/2022.
//

#ifndef SABT_VEC3I_H
#define SABT_VEC3I_H


struct Vec3i {
public:
    long long x, y, z;

    Vec3i();
    Vec3i(long long x, long long y, long long z);

    const Vec3i operator*(long long factor);
};


#endif //SABT_VEC3I_H
