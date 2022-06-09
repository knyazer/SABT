//
// Created by knyaz on 6/8/2022.
//
// Triplet is an intermediate representation of indices in octrees. Allows to transform coordinates to indices and vice versa.
//

#ifndef SABT_TRIPLET_H
#define SABT_TRIPLET_H

#include <cstdint>
#include <stdexcept>
#include <cassert>
#include <Vec3i.h>

constexpr uint8_t X_INDEX = 0;
constexpr uint8_t Y_INDEX = 1;
constexpr uint8_t Z_INDEX = 2;

constexpr uint8_t X_MASK = (1 << X_INDEX);
constexpr uint8_t Y_MASK = (1 << Y_INDEX);
constexpr uint8_t Z_MASK = (1 << Z_INDEX);

constexpr uint8_t INDEX_MASK = X_MASK + Y_MASK + Z_MASK;

class Triplet {
private:
    uint8_t data;
public:
    Triplet() { data = 0; }
    Triplet(int dx, int dy, int dz) { set(dx, dy, dz); }
    Triplet(int index) { set(index); }
    Triplet(Vec3i pos) { set(pos); }

    // Such a strange construct needed to avoid excessive computation (subtraction costs a lot)
    inline int x() { return ((data & X_MASK) << 1) - 1; }
    inline int y() { return ((data & Y_MASK)) - 1; }
    inline int z() { return ((data & Z_MASK) >> 1) - 1; }

    inline int index() { return data & INDEX_MASK; }

    inline void set(int x, int y, int z) {
        data =  ((x >= 0 ? 1 : 0) << X_INDEX) +
                ((y >= 0 ? 1 : 0) << Y_INDEX) +
                ((z >= 0 ? 1 : 0) << Z_INDEX);
    }

    inline void set(Vec3i pos) {
        set(pos.x, pos.y, pos.z);
    }

    inline void set(int index) {
        if (index < 0 || index > 7)
            throw std::runtime_error("Incorrect value for the triplet index.");
        data = index;
    }

    inline Triplet reverse() {
        return {-x(), -y(), -z()};
    }

    Triplet(const Triplet& other) = default;
    ~Triplet() = default;
};


#endif //SABT_TRIPLET_H
