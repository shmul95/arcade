/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IGraphics.hpp
*/

#pragma once

#include "CellContent.hpp"
#include "Event.hpp"
#include <string>
#include <vector>

namespace Arcade {

class IGraphics
{
    public:
        virtual ~IGraphics() = default;

        virtual std::vector<Event> handleInput() = 0;
        virtual void render(
            const std::vector<std::vector<Cell>> &map,
            int score, const std::string &playerName,
            const std::vector<std::pair<std::string, int>> &highScores
        ) = 0;

        virtual void clear() = 0;
        virtual std::string getName() const = 0;
        virtual void close() = 0;
};
    // Factory function
    extern "C" IGraphics *createGraphics();
    extern "C" void destroyGraphics(IGraphics *graphics);
}// namespace Arcade
