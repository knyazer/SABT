//
// Created by knyaz on 20/07/22.
//

#ifndef SABT_STATE_COLOR_H
#define SABT_STATE_COLOR_H

#include <stdexcept>

using ch_t = unsigned char;

class Color {
public:
    enum BaseColor {
        WHITE = 0,
        BLACK = 1,
        RED = 2,
        BLUE = 3,
        GREEN = 4,
        GRAY = 5
    };

    ch_t r, g, b;

    Color();

    Color(ch_t red, ch_t green, ch_t blue);

    Color(BaseColor color);

    Color& operator=(BaseColor color);
};

#endif //SABT_STATE_COLOR_H
