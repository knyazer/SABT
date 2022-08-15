//
// Created by knyaz on 15/08/22.
//

#include "include/Color.h"

Color::Color() {
    r = 0; g = 0; b = 0;
}

Color::Color(ch_t red, ch_t green, ch_t blue) {
    r = red;
    g = green;
    b = blue;
}

Color::Color(BaseColor color) {
    operator=(color);
}

Color& Color::operator=(BaseColor color) {
    switch (color) {
        case WHITE:
            r = 255; g = 255; b = 255;
            break;

        case BLACK:
            r = 0; g = 0; b = 0;
            break;

        case GRAY:
            r = 128; g = 128; b = 128;
            break;

        case RED:
            r = 255; g = 0; b = 0;
            break;

        case GREEN:
            r = 0; g = 255; b = 0;
            break;

        case BLUE:
            r = 0; g = 0; b = 255;
            break;

        default:
            throw std::runtime_error("Not able to process this value of BaseColor");
    }

    return *this;
}
