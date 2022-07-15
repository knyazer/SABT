//
// Created by knyaz on 15/07/22.
//

#ifndef SABT_COLOR_H
#define SABT_COLOR_H

#include <cstdint>

namespace graphics {
    struct Color {
        uint8_t r, g, b;

        Color() = default;

        Color(uint8_t r, uint8_t g, uint8_t b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }
    };

    const Color RED = Color(0xFF, 0, 0);
    const Color GREEN = Color(0, 0xFF, 0);
    const Color BLUE = Color(0, 0, 0xFF);
    const Color WHITE = Color(0xFF, 0xFF, 0xFF);
    const Color BLACK = Color(0, 0, 0);
    const Color GRAY = Color(0x88, 0x88, 0x88);
}

#endif //SABT_COLOR_H
