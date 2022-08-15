//
// Created by knyaz on 15/07/22.
//

#ifndef SABT_COLOR_H
#define SABT_COLOR_H

#include <cstdint>
#include <SABT.h>

namespace graphics {
    struct ColorRGB {
        uint8_t r, g, b;

        ColorRGB() = default;

        ColorRGB(uint8_t r, uint8_t g, uint8_t b) {
            this->r = r;
            this->g = g;
            this->b = b;
        }

        ColorRGB(Color color) {
            r = color.r;
            g = color.g;
            b = color.b;
        }

        ColorRGB(Color::BaseColor color) {
            Color rgb(color);

            r = rgb.r;
            g = rgb.g;
            b = rgb.b;
        }
    };
}

#endif //SABT_COLOR_H
