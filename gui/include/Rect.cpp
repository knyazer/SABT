//
// Created by knyaz on 15/07/22.
//

#include "Rect.h"

graphics::Rect::Rect(double x, double y, double width, double height) : SDL_Rect() {
    this->x = static_cast<int>(round(x));
    this->y = static_cast<int>(round(y));
    w = static_cast<int>(round(width));
    h = static_cast<int>(round(height));
}

graphics::Rect::Rect(int x, int y, int width, int height) : SDL_Rect() {
    this->x = x;
    this->y = y;
    w = width;
    h = height;
}
