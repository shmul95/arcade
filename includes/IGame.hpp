/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** IGame.hpp
*/

#pragma once

#include "CellContent.hpp"
#include "Event.hpp"
#include <string>
#include <vector>

namespace Arcade {

struct MenuSelection {
        bool selectionMade = false;
        int selectedGameIndex = -1;
        int selectedGraphicsIndex = -1;
};

class IGame
{
    public:
        virtual ~IGame() = default;
        virtual void update(const std::vector<Event> &events) = 0;
        virtual std::vector<std::vector<Cell>> getMap() const = 0;
        virtual std::string getName() const = 0;
        virtual int getScore() const = 0;
        virtual bool isGameOver() const = 0;
        virtual void restart() = 0;
        virtual void close() = 0;

        virtual MenuSelection getSelection() const = 0;
        virtual void setAvailableLibraries([[maybe_unused]] const std::vector<std::string> &gameLibs,
            [[maybe_unused]] const std::vector<std::string> &graphicsLibs) = 0;
        virtual bool wantsTextInput() const = 0;
        virtual void setPlayerName([[maybe_unused]] const std::string &name) = 0;
};

// Factory function
extern "C" IGame *createGame();
extern "C" void destroyGame(IGame *game);

}// namespace Arcade