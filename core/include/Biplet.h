//
// Created by knyaz on 6/8/2022.


#ifndef SABT_BIPLET_H
#define SABT_BIPLET_H

#include <cstdint>
#include <stdexcept>
#include <cassert>

/**
 * Triplet is an intermediate representation of indices in octrees.
 * Allows to transform coordinates to indices and vice versa.
 */
class Biplet {
private:
    uint8_t data{};

    static constexpr uint8_t X_INDEX = 0;
    static constexpr uint8_t Y_INDEX = 1;

    static constexpr uint8_t X_MASK = (1 << X_INDEX);
    static constexpr uint8_t Y_MASK = (1 << Y_INDEX);

    static constexpr uint8_t INDEX_MASK = X_MASK + Y_MASK;
public:
    Biplet() { data = 0; }

    Biplet(int dx, int dy) { set(dx, dy); }

    Biplet(int index) { set(index); }

    /// Returns the x coordinate of the biplet (0 or 1)
    [[nodiscard]] inline int x() const { return (data & X_MASK); }

    /// Returns the y coordinate of the biplet (0 or 1)
    [[nodiscard]] inline int y() const { return (data & Y_MASK) >> 1; }

    /// Returns the index of the biplet, the index of the node with given coordinates
    [[nodiscard]] inline int index() const { return data & INDEX_MASK; }

    /// Sets the child by given x, y. All either 0 or 1
    inline void set(int x, int y) {
        if (!((x == 0 || x == 1) && (y == 0 || y == 1)))
            throw std::runtime_error("Not allowed to set triplet with anything except 0-1 for each coordinate.");

        data = (x << X_INDEX) +
               (y << Y_INDEX);
    }

    /// Set the triplet by index of the node
    inline void set(int index) {
        if (index < 0 || index > 7)
            throw std::runtime_error("Incorrect value for the triplet index.");
        data = index;
    }

    /// Inverses the coordinates of the current Triplet, and returns the reversed one.
    /// idk why function is called reverse
    [[nodiscard]] inline Biplet reverse() const {
        return {1 - x(), 1 - y()};
    }

    friend std::ostream &operator<<(std::ostream &os, const Biplet &val);

    Biplet(const Biplet &other) = default;

    ~Biplet() = default;
};


#endif //SABT_BIPLET_H
