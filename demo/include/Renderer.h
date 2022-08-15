//
// Created by knyaz on 7/3/2022.
//

#ifndef SABT_RENDERER_H
#define SABT_RENDERER_H

#include <iostream>
#include <memory>
#include <string>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Color.h"
#include "Rect.h"
#include "GetPath.h"

using std::unique_ptr;
using std::shared_ptr;
using std::string;

namespace graphics {
    class Renderer {
    private:
        SDL_Window* window;
        SDL_Surface* surface;
        SDL_Renderer* renderer;

        bool mousePressed, mouseUp;

    public:

        struct Pos2 {
            int x, y;
        };

        Pos2 mouse, center, delta;

        static const size_t KEY_ARR_SIZE = 128;
        bool pressed[KEY_ARR_SIZE];

        double fps;

        bool debug;

        Renderer();

        void createWindow(const string& name, Rect rect);

        void drawRect(Rect rect, ColorRGB color);

        void fillRect(Rect rect, ColorRGB color);

        void clear(ColorRGB color);

        void renderFPS();

        bool update();

        ~Renderer();

        Pos2 getMouseDelta();

        void enableDebugging();
    };
}

#endif //SABT_RENDERER_H
