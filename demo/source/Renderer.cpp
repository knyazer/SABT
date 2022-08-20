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

    debug = false;

    fps = 0;

    for (bool &i: pressed)
        i = false;

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
        throw std::runtime_error("Failed to initialize sdl2");
    }

    if (TTF_Init() == -1) {
        throw std::runtime_error("Failed to initialize TTF");
    }
}

void graphics::Renderer::createWindow(const string &name, graphics::Rect rect) {
    SDL_CreateWindowAndRenderer(rect.w, rect.h,0,&window,&renderer);

    if (window == nullptr) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        throw std::runtime_error("Unable to create window");
    }

    surface = SDL_GetWindowSurface(window);

    center = {rect.w / 2, rect.h / 2};
    SDL_ShowCursor(1);
}

void graphics::Renderer::clear(ColorRGB color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderClear(renderer);
}


void graphics::Renderer::fillRect(graphics::Rect rect, ColorRGB color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderFillRect(renderer, &rect);
}

void graphics::Renderer::drawRect(graphics::Rect rect, ColorRGB color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderDrawRect(renderer, &rect);
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
    // fps
    static auto startTime = SDL_GetTicks64();
    static double avgFPS = 10;

    auto frameTime = SDL_GetTicks64() - startTime;
    // complementary filter to calculate average fps
    avgFPS = 0.05 * (frameTime > 0 ? static_cast<double>(1000.0 / frameTime) : 0) + 0.95 * avgFPS;
    startTime = SDL_GetTicks64();

    // change the drawn value infrequently
    static int _counter = 0;
    if (_counter++ % 5 == 0) {
        _counter = 1;
        fps = avgFPS;
    }

    renderFPS();

    // delay
    SDL_RenderPresent(renderer);

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

graphics::Renderer::~Renderer() {
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

void graphics::Renderer::renderFPS() {
    static TTF_Font* font = TTF_OpenFont((getPath() + "/font/ShareTechMono-Regular.ttf").c_str(), 50);

    if (font == nullptr)
        std::cerr << " Failed to load font : " << SDL_GetError() << std::endl;

    SDL_Color White = {0, 128, 255};

    std::string num = std::to_string(static_cast<unsigned>(fps));
    std::string str = std::string(3 - std::min(3, static_cast<int>(num.length())), '0') + num;
    SDL_Surface* surfaceMessage = TTF_RenderText_Blended(font, str.c_str(), White);

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

    SDL_Rect Message_rect;
    Message_rect.x = 0;
    Message_rect.y = 0;
    Message_rect.w = 140;
    Message_rect.h = 50;

    SDL_RenderCopy(renderer, Message, nullptr, &Message_rect);
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
}

void graphics::Renderer::enableDebugging() {
    debug = true;
}