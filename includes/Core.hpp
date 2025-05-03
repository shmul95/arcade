/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** Core
*/

#pragma once

#include "DLLoader.hpp"
#include "IGame.hpp"
#include "IGraphics.hpp"
#include <unordered_map>
#include <iostream>
#include <filesystem>

#define LIB_DIR "lib/"

void handle_error(const char *msg);

namespace Arcade
{
        class Core
    {
    private:
        std::unordered_map<std::string, DLLoader<IGame>> _gameLoaders;
        std::unordered_map<std::string, DLLoader<IGraphics>> _graphicLoaders;

        std::vector<std::string> _gamesNames;
        std::vector<std::string> _graphicsNames;

        int _gameIdx;
        int _graphicIdx;

        void open_graphical_lib(const std::filesystem::directory_entry &entry);

        void handle_higher_order_events(
            std::vector<Arcade::Event> event,
            std::unique_ptr<IGame> &game_inst,
            std::unique_ptr<IGraphics> &graphics_inst
        );

        void switch_graphics_library(std::unique_ptr<IGraphics> &graphics);
        void switch_game_library(std::unique_ptr<IGame> &game);

    public:
        Core(const std::string &defaultGraphicalLib);
        ~Core();

        void run(void);
    };
} // namespace Arcade
