//
// Created by knyaz on 15/07/22.
//

#ifndef SABT_COLOR_H
#define SABT_COLOR_H

#include <cstdint>

namespace graphics {
    struct ColorSDL {
        uint8_t r, g, b;

        ColorSDL() = default;

        ColorSDL(uint8_t r, uint8_t g, uint8_t b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }

    };

    namespace colors {
        const ColorSDL RED = ColorSDL(0xFF, 0, 0);
        const ColorSDL GREEN = ColorSDL(0, 0xFF, 0);
        const ColorSDL BLUE = ColorSDL(0, 0, 0xFF);
        const ColorSDL WHITE = ColorSDL(0xFF, 0xFF, 0xFF);
        const ColorSDL BLACK = ColorSDL(0, 0, 0);
        const ColorSDL GRAY = ColorSDL(0x88, 0x88, 0x88);
    }
}

#endif //SABT_COLOR_H
