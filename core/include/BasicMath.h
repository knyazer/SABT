//
// Created by knyaz on 12/08/22.
//

#ifndef SABT_BASICMATH_H
#define SABT_BASICMATH_H

template <typename T1, typename T2>
T1 max2(T1 a, T2 b) {
    return a > b ? a : b;
}

template <typename T1, typename T2>
T1 min2(T1 a, T2 b) {
    return a < b ? a : b;
}

template <typename T>
int sign(T x) {
    return x < 0 ? -1 : x == 0 ? 0 : 1;
}

template <typename T>
T abs(T x) {
    return x < 0 ? -x : x;
}

template <typename T>
T sq(T x) {
    return x * x;
}

#endif //SABT_BASICMATH_H
