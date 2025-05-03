/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** NCurses
*/

#pragma once

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>

#include "IGraphics.hpp"
#include "Event.hpp"
#include "CellContent.hpp"

#include "ncurses.h"

namespace Arcade
{
    class NCurses : public IGraphics
    {
    private:
        std::string _name;
        std::unordered_map<Color, int> _colors;
    public:
        NCurses();
        ~NCurses();

        std::vector<Event> handleInput();
        void render(
            const std::vector<std::vector<Cell>> &map,
            int score, const std::string &playerName,
            const std::vector<std::pair<std::string, int>> &highScores
        );

        std::string getName() const { return this->_name; }
        void clear() { ::clear(); }
        void close() { endwin(); }
    };
    extern "C" IGraphics *createGraphics();
    extern "C" void destroyGraphics(IGraphics *graphics);
} // namespace Arcade

