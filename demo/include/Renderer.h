//
// Created by knyaz on 7/3/2022.
//

#ifndef SABT_RENDERER_H
#define SABT_RENDERER_H

#include <iostream>
#include <memory>
#include <string>
#include <SDL2/SDL.h>
#include <Color.h>
#include <Rect.h>

using std::unique_ptr;
using std::shared_ptr;
using std::string;

namespace graphics {
    class Renderer {
    private:
        SDL_Window* window;
        SDL_Surface* surface;

    public:

        struct Pos2 {
            int x, y;
        };

        Pos2 mouse, center;

        static const size_t KEY_ARR_SIZE = 128;
        bool pressed[KEY_ARR_SIZE];

        Renderer();

        void createWindow(const string& name, Rect rect);

        void drawRect(Rect rect, Color color);

        void clear(Color color);

        bool update();

        ~Renderer();

        Pos2 getMouseDelta();
    };
}

#endif //SABT_RENDERER_H