//
// Created by knyaz on 15/07/22.
//

#ifndef SABT_RECT_H
#define SABT_RECT_H

#include <SDL2/SDL.h>
#include <cstdint>

namespace graphics {
    class Rect : public SDL_Rect {
    public:
        Rect(double x, double y, double width, double height);

        Rect(int x, int y, int width, int height);
    };
}


#endif //SABT_RECT_H
