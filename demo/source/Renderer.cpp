//
// Created by knyaz on 7/3/2022.
//

#include <Renderer.h>

graphics::Renderer::Renderer() {
    window = nullptr;
    surface = nullptr;

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
}

void graphics::Renderer::clear(Color color) {
    SDL_FillRect(surface, nullptr, SDL_MapRGB(surface->format, color.r, color.g, color.b));
}

void graphics::Renderer::update() {
    SDL_UpdateWindowSurface(window);
    SDL_Delay(10);
}

void graphics::Renderer::drawRect(graphics::Rect rect, Color color) {
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, color.r, color.g, color.b));
}

graphics::Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool graphics::Renderer::isStopped() {
    SDL_Event e;
    return SDL_PollEvent(&e) && (e.type == SDL_QUIT || (e.type == SDL_KEYDOWN && e.key.keysym.scancode ==
                                                                                         SDL_SCANCODE_ESCAPE));
}
