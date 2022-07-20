//
// Created by knyaz on 7/3/2022.
//

#include <Renderer.h>

graphics::Renderer::Renderer() {
    window = nullptr;
    surface = nullptr;

    mouse = {0, 0};
    center = {0, 0};

    mousePressed = false;
    mouseUp = false;

    for (bool &i: pressed)
        i = false;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        throw std::runtime_error("Failed to initialize sdl2");
    }
}

void graphics::Renderer::createWindow(const string &name, graphics::Rect rect) {
    window = SDL_CreateWindow(
            name.c_str(),
            rect.x, rect.y,
            rect.w, rect.h,
            SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        throw std::runtime_error("Unable to create window");
    }

    surface = SDL_GetWindowSurface(window);

    center = {rect.w / 2, rect.h / 2};
    SDL_ShowCursor(1);
}

void graphics::Renderer::clear(ColorSDL color) {
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, color.r, color.g, color.b));
}

graphics::Renderer::Pos2 graphics::Renderer::getMouseDelta() {
    static Pos2 prev = {mouse.x, mouse.y};
    static bool reset = true;

    if (mouseUp) {
        reset = true;
        mouseUp = false;
    }

    if (mousePressed) {
        if (reset) {
            prev = {mouse.x, mouse.y};
            reset = false;
        }

        Pos2 res = {mouse.x - prev.x, mouse.y - prev.y};
        prev = {mouse.x, mouse.y};

        return res;
    }
    else {
        return {0, 0};
    }
}

bool graphics::Renderer::update() {
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);


    // mouse
    SDL_PumpEvents();
    SDL_GetMouseState(&mouse.x, &mouse.y);

    // keyboard + quit
    for (bool &i: pressed)
        i = false;

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT: {
                return false;
            }

            case SDL_KEYDOWN: {
                uint16_t key = e.key.keysym.scancode;
                if (key == SDL_SCANCODE_ESCAPE)
                    return false;

                if (key >= SDL_SCANCODE_A && key <= SDL_SCANCODE_Z)
                    pressed[(key - SDL_SCANCODE_A) + 'a'] = true;

                if (key >= SDL_SCANCODE_1 && key <= SDL_SCANCODE_9)
                    pressed[(key - SDL_SCANCODE_1) + '1'] = true;
                break;
            }

            case SDL_MOUSEBUTTONDOWN: {
                mousePressed = true;
                break;
            }

            case SDL_MOUSEBUTTONUP: {
                mousePressed = false;
                mouseUp = true;
                break;
            }

            default: {
                break;
            }
        }
    }

    return true;
}

void graphics::Renderer::drawRect(graphics::Rect rect, ColorSDL color) {
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, color.r, color.g, color.b));
}

graphics::Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
