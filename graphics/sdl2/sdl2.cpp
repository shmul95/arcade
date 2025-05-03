/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** sdl2
*/

#include "sdl2.hpp"
#include <cmath>

namespace Arcade {
    ArcadeSDL2::ArcadeSDL2() {
        SDL_DisplayMode dm;
        SDL_Init(SDL_INIT_VIDEO);
        TTF_Init();
        SDL_GetDesktopDisplayMode(0, &dm);
        _window = SDL_CreateWindow("SDL2", 
                                 SDL_WINDOWPOS_CENTERED,
                                 SDL_WINDOWPOS_CENTERED,
                                 dm.w, dm.h,
                                 SDL_WINDOW_SHOWN);
        _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
        _font = TTF_OpenFont("assets/fonts/Arial.ttf", 24);

        _keyMapping = {
            {UP, SDLK_UP},
            {DOWN, SDLK_DOWN},
            {LEFT, SDLK_LEFT},
            {RIGHT, SDLK_RIGHT},
            {SPACE, SDLK_SPACE},
            {ENTER, SDLK_RETURN},
            {ESCAPE, SDLK_ESCAPE},
            {NEXT_GAME, SDLK_n},
            {PREV_GAME, SDLK_p},
            {NEXT_GRAPHICS, SDLK_g},
            {PREV_GRAPHICS, SDLK_h},
            {RESTART_GAME, SDLK_r},
            {QUIT, SDLK_q}
        };
    }

    ArcadeSDL2::~ArcadeSDL2() {
        TTF_CloseFont(_font);
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        TTF_Quit();
        SDL_Quit();
    }

    std::vector<Event> ArcadeSDL2::handleInput() {
        std::vector<Event> events;
        SDL_Event sdlEvent;
        
        while (SDL_PollEvent(&sdlEvent)) {
            switch (sdlEvent.type) {
                case SDL_QUIT:
                    events.emplace_back(QUIT);
                    break;
                    
                case SDL_KEYDOWN:
                    for (const auto& pair : _keyMapping) {
                        if (sdlEvent.key.keysym.sym == pair.second) {
                            events.emplace_back(pair.first);
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    MouseButton btn = NONE;
                    switch (sdlEvent.button.button) {
                        case SDL_BUTTON_LEFT: btn = LEFT_CLICK; break;
                        case SDL_BUTTON_RIGHT: btn = RIGHT_CLICK; break;
                        case SDL_BUTTON_MIDDLE: btn = MIDDLE_CLICK; break;
                    }
                    events.emplace_back(btn, sdlEvent.button.x, sdlEvent.button.y);
                    break;
            }
        }
        return events;
    }

    void ArcadeSDL2::render(const std::vector<std::vector<Cell>>& map, int score,
                           const std::string& playerName,
                           const std::vector<std::pair<std::string, int>>& highScores) {
        SDL_Delay(100);
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
        
        for (const auto& row : map) {
            for (const auto& cell : row) {
                draw_cell(cell);
            }
        }

        SDL_Color white = {255, 255, 255, 255};
        SDL_Color yellow = {255, 255, 0, 255};
        int uiX = map[0].size() * 21;
        int uiY = 10;
        
        draw_text("Score: " + std::to_string(score), uiX, uiY, white);
        uiY += 40;
        draw_text("Controls:", uiX, uiY, white);
        uiY += 30;
        draw_text("Move: Arrow Keys", uiX, uiY, yellow);
        uiY += 25;
        draw_text("Next Game: N", uiX, uiY, yellow);
        uiY += 25;
        draw_text("Prev Game: P", uiX, uiY, yellow);
        uiY += 25;
        draw_text("Next Graphics: G", uiX, uiY, yellow);
        uiY += 25;
        draw_text("Prev Graphics: H", uiX, uiY, yellow);
        uiY += 25;
        draw_text("Restart: R", uiX, uiY, yellow);
        uiY += 25;
        draw_text("Quit: Q or ESC", uiX, uiY, yellow);
        SDL_RenderPresent(_renderer);
    }

    void ArcadeSDL2::draw_cell(const Cell& cell) {
        SDL_Color sdlColor;
        switch (cell.color) {
            case WHITE: sdlColor = {255, 255, 255, 255}; break;
            case RED: sdlColor = {255, 0, 0, 255}; break;
            case GREEN: sdlColor = {0, 255, 0, 255}; break;
            case BLUE: sdlColor = {0, 0, 255, 255}; break;
            case YELLOW: sdlColor = {255, 255, 0, 255}; break;
            case PURPLE: sdlColor = {128, 0, 128, 255}; break;
            case ORANGE: sdlColor = {255, 165, 0, 255}; break;
            case BLACK: sdlColor = {0, 0, 0, 255}; break;
            default: sdlColor = {128, 128, 128, 0};
        }
        
        int x = cell.x * 20;
        int y = cell.y * 20;
        switch (cell.type) {
            case BLOCK: {
                SDL_Rect rect{x, y, 20, 20};
                SDL_SetRenderDrawColor(_renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
                SDL_RenderFillRect(_renderer, &rect);
                break;
            }
            case PLAYER: {
                draw_circle(x + 10, y + 10, 10, sdlColor);
                SDL_Color black = {0, 0, 0, 255};
                draw_circle(x + 10, y + 10, 8, black);
                break;
            }
            case ENEMY: {
                draw_circle(x + 10, y + 10, 10, sdlColor);
                break;
            }
            case FOOD: {
                draw_circle(x + 10, y + 10, 10, sdlColor);
                SDL_Color black = {0, 0, 0, 255};
                draw_circle(x + 10, y + 10, 8, black);
                break;
            }
            case POWERUP: {
                draw_circle(x + 10, y + 10, 10, sdlColor);
                break;
            }
            case PROJECTILE: {
                draw_circle(x + 10, y + 10, 5, sdlColor);
                break;
            }
            case OBSTACLE: {
                SDL_Rect rect{x, y, 20, 20};
                SDL_SetRenderDrawColor(_renderer, sdlColor.r, sdlColor.g, sdlColor.b, sdlColor.a);
                SDL_RenderFillRect(_renderer, &rect);
                break;
            }
            default:
                break;
        }
    }

    void ArcadeSDL2::draw_circle(int centerX, int centerY, int radius, SDL_Color color) {
        SDL_SetRenderDrawColor(_renderer, color.r, color.g, color.b, color.a);
        for (int w = 0; w < radius * 2; w++) {
            for (int h = 0; h < radius * 2; h++) {
                int dx = radius - w;
                int dy = radius - h;
                if ((dx*dx + dy*dy) <= (radius * radius)) {
                    SDL_RenderDrawPoint(_renderer, centerX + dx, centerY + dy);
                }
            }
        }
    }

    void ArcadeSDL2::draw_text(const std::string& text, int x, int y, SDL_Color color) {
        SDL_Surface* surface = TTF_RenderText_Solid(_font, text.c_str(), color);
        SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, surface);
        
        SDL_Rect destRect{x, y, surface->w, surface->h};
        SDL_RenderCopy(_renderer, texture, nullptr, &destRect);
        
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }

    std::string ArcadeSDL2::getName() const {
        return "SDL2";
    }

    void ArcadeSDL2::clear() {
        SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
        SDL_RenderClear(_renderer);
    }

    void ArcadeSDL2::close() {
        SDL_DestroyRenderer(_renderer);
        SDL_DestroyWindow(_window);
        TTF_Quit();
        SDL_Quit();
    }
    extern "C" IGraphics* createGraphics() { return new ArcadeSDL2(); }
    extern "C" void destroyGraphics(IGraphics* graphics) { delete graphics; }
}
