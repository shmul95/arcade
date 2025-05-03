/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** sdl2
*/

#pragma once
#include "../../includes/IGraphics.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>

namespace Arcade {
    class ArcadeSDL2 : public IGraphics {
        public:
            ArcadeSDL2();
            ~ArcadeSDL2();

            std::vector<Event> handleInput() override;
            void render(const std::vector<std::vector<Cell>>& map, int score,
                       const std::string& playerName,
                       const std::vector<std::pair<std::string, int>>& highScores) override;
            void clear() override;
            std::string getName() const override;
            void close() override;

        private:
            SDL_Window* _window;
            SDL_Renderer* _renderer;
            TTF_Font* _font;
            std::unordered_map<KeyType, SDL_Keycode> _keyMapping;

            void draw_cell(const Cell& cell);
            void draw_text(const std::string& text, int x, int y, SDL_Color color);
            void draw_circle(int centerX, int centerY, int radius, SDL_Color color);
    };

    extern "C" IGraphics* createGraphics();
    extern "C" void destroyGraphics(IGraphics* graphics);
}
