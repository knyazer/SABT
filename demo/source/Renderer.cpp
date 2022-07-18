//
// Created by knyaz on 7/3/2022.
//

#include <Renderer.h>

graphics::Renderer::Renderer() {
    window = nullptr;
    surface = nullptr;

    mouse = {0, 0};
    center = {0, 0};

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
    SDL_ShowCursor(0);
    SDL_WarpMouseInWindow(window, center.x, center.y);
}

void graphics::Renderer::clear(Color color) {
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, color.r, color.g, color.b));
}

graphics::Renderer::Pos2 graphics::Renderer::getMouseDelta() {
    SDL_WarpMouseInWindow(window, center.x, center.y);

    return {mouse.x - center.x, mouse.y - center.y};
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

            default: {
                break;
            }
        }
    }

    return true;
}

void graphics::Renderer::drawRect(graphics::Rect rect, Color color) {
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, color.r, color.g, color.b));
}

graphics::Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}
