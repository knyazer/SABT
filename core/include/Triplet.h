//
// Created by knyaz on 6/8/2022.
//
// Triplet is an intermediate representation of indices in octrees. Allows to transform coordinates to indices and vice versa.
//

#ifndef SABT_TRIPLET_H
#define SABT_TRIPLET_H

#include "Vec3i.h"

#include <cstdint>
#include <stdexcept>
#include <cassert>

constexpr uint8_t X_INDEX = 0;
constexpr uint8_t Y_INDEX = 1;
constexpr uint8_t Z_INDEX = 2;

constexpr uint8_t X_MASK = (1 << X_INDEX);
constexpr uint8_t Y_MASK = (1 << Y_INDEX);
constexpr uint8_t Z_MASK = (1 << Z_INDEX);

constexpr uint8_t INDEX_MASK = X_MASK + Y_MASK + Z_MASK;

class Triplet {
private:
    uint8_t data{};
public:
    Triplet() { data = 0; }

    Triplet(int dx, int dy, int dz) { set(dx, dy, dz); }

    Triplet(int index) { set(index); }

    Triplet(Vec3i pos) { set(pos); }

    // Returns the x coordinate of the triplet (0 or 1)
    [[nodiscard]] inline int x() const { return (data & X_MASK); }

    // Returns the y coordinate of the triplet (0 or 1)
    [[nodiscard]] inline int y() const { return (data & Y_MASK) >> 1; }

    // Return the z coordinate of the triplet (0 or 1)
    [[nodiscard]] inline int z() const { return (data & Z_MASK) >> 2; }

    // Returns the coordinates of the triplet as Vec3i ( complete vector )
    [[nodiscard]] inline Vec3i vec() const { return {x(), y(), z()}; }

    // Returns the index of the triplet, the index of the node with given coordinates
    [[nodiscard]] inline int index() const { return data & INDEX_MASK; }

    // Sets the child by given x, y and z. All either 0 or 1
    inline void set(int x, int y, int z) {
        if (!((x == 0 || x == 1) && (y == 0 || y == 1) && (z == 0 || z == 1)))
            throw std::runtime_error("Not allowed to set triplet with anything except 0-1 for each coordinate.");

        data = (x << X_INDEX) +
               (y << Y_INDEX) +
               (z << Z_INDEX);
    }

    // Sets the triplet by the Vec3i. If coord > 0 then triplet.coord = 1, otherwise triplet.coord = 0.
    // Similar to the Vec3i::sign
    inline void set(Vec3i pos) {
        set(pos.x > 0 ? 1 : 0, pos.y > 0 ? 1 : 0, pos.z > 0 ? 1 : 0);
    }

    // Set the triplet by index of the node
    inline void set(int index) {
        if (index < 0 || index > 7)
            throw std::runtime_error("Incorrect value for the triplet index.");
        data = index;
    }

    // Inverses the coordinates of the current Triplet, and returns the inversed one.
    // idk why function is called reverse
    [[nodiscard]] inline Triplet reverse() const {
        return {1 - x(), 1 - y(), 1 - z()};
    }

    friend std::ostream &operator<<(std::ostream &os, const Triplet &val);

    Triplet(const Triplet &other) = default;

    ~Triplet() = default;
};


#endif //SABT_TRIPLET_H
