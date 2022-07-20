//
// Created by knyaz on 15/07/22.
//

#ifndef SABT_COLOR_H
#define SABT_COLOR_H

#include <cstdint>
#include <SABT.h>

namespace graphics {
    static uint32_t toSDL[] = {
            0xffffff,
            0x000000,
            0xff0000,
            0x0000ff,
            0x00ff00,
            0x666666,
    };

    struct ColorSDL {
        uint8_t r, g, b;

        ColorSDL() = default;

        ColorSDL(uint8_t r, uint8_t g, uint8_t b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        ColorSDL(Color color) {
            uint32_t encoded = toSDL[color];

            r = (encoded >> 16) & 0xff;
            g = (encoded >> 8) & 0xff;
            b = encoded & 0xff;
        }

    };
}

#endif //SABT_COLOR_H
