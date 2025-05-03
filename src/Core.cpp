/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** Core
*/

#include "Core.hpp"

const std::vector<std::string> graphical_lib = {
    "arcade_ncurses.so",
    "arcade_sdl2.so",
    "arcade_ndk++.so",
    "arcade_aalib.so",
    "arcade_libcaca.so",
    "arcade_allegro5.so",
    "arcade_xlib.so",
    "arcade_gtk+.so",
    "arcade_sfml.so",
    "arcade_irrlicht.so",
    "arcade_opengl.so",
    "arcade_vulkan.so",
    "arcade_qt5.so"
};

namespace Arcade
{
    static bool word_in_warr(std::string word, std::vector<std::string> warr)
    {
        for (std::string arr_word : warr)
            if (word == arr_word)
                return true;
        return false;
    }

    void Core::open_graphical_lib(const std::filesystem::directory_entry &entry)
    {
        std::string filename = entry.path().filename().string();
        if (filename.rfind("arcade_", 0) == 0 and filename.size() >= 3
        and filename.substr(filename.size() - 3) == ".so") {
            std::string libName = LIB_DIR + filename;
            if (word_in_warr(filename, graphical_lib)) {
                _graphicLoaders.emplace(libName, libName);
                _graphicsNames.push_back(libName);
                // try {
                //     _graphicLoaders.try_emplace(libName, libName);
                //     _graphicsNames.push_back(libName);
                // } catch (const std::exception& e) {
                //     std::cerr << "Error loading graphics library " << libName << ": " << e.what() << std::endl;
                // }
            } else {
                _gameLoaders.emplace(libName, libName);
                _gamesNames.push_back(libName);
                // try {
                //     _gameLoaders.try_emplace(libName, libName);
                //     _gamesNames.push_back(libName);
                // } catch (const std::exception& e) {
                //     std::cerr << "Error loading game library " << libName << ": " << e.what() << std::endl;
                // }
            }
        }
    }

    static int find_lib_idx(std::string name, std::vector<std::string> list)
    {
        size_t list_size = list.size();

        for (size_t i = 0; i < list_size; i++) {
            if (list.at(i) == name)
                return i;
        }
        return -1;
    }

    Core::Core(const std::string &defaultGraphicalLib)
        : _gameIdx(0), _graphicIdx(0)
    {
        for (const auto &entry : std::filesystem::directory_iterator(LIB_DIR))
            if (entry.is_regular_file())
                open_graphical_lib(entry);

        _gameIdx = find_lib_idx("lib/arcade_minesweeper.so", _gamesNames);
        _graphicIdx = find_lib_idx(defaultGraphicalLib, _graphicsNames);
    }

    Core::~Core() {}

    void Core::switch_graphics_library(std::unique_ptr<IGraphics> &graphics)
    {
        std::string newGraphicsLib = _graphicsNames.at(_graphicIdx);
        std::cout << "Switching to graphics library: " << newGraphicsLib << std::endl;

        graphics.reset(
            _graphicLoaders.at(
                _graphicsNames.at(_graphicIdx)
            ).getInstance()
        );

        if (!graphics)
            std::cerr << "Failed to load graphics library: " << newGraphicsLib << std::endl;
    }

    void Core::switch_game_library(std::unique_ptr<IGame> &game)
    {
        std::string newGameLib = _gamesNames.at(_gameIdx);
        std::cout << "Switching to game library: " << newGameLib << std::endl;

        game.reset(
            _gameLoaders.at(
                _gamesNames.at(_gameIdx)
            ).getInstance()
        );

        if (!game)
            std::cerr << "Failed to load game library: " << newGameLib << std::endl;
    }

    void Core::handle_higher_order_events(
        std::vector<Arcade::Event> event,
        std::unique_ptr<IGame> &game,
        std::unique_ptr<IGraphics> &graphics
    ) {
        if (not event.empty()) {
            auto act_event = event.front().key;
            size_t game_size = _gamesNames.size();
            size_t graphic_size = _graphicsNames.size();

            if (act_event == Arcade::NEXT_GAME) {
                _gameIdx = (_gameIdx + 1) % game_size;
                switch_game_library(game);    
            } else if (act_event == Arcade::PREV_GAME) {
                _gameIdx = _gameIdx == 0 ? _gameIdx - 1 + game_size : _gameIdx - 1;
                switch_game_library(game);    
            } else if (act_event == Arcade::NEXT_GRAPHICS) {
                _graphicIdx = (_graphicIdx + 1) % graphic_size;
                switch_graphics_library(graphics);
            } else if (act_event == Arcade::PREV_GRAPHICS) {
                _graphicIdx = _graphicIdx == 0 ? _graphicIdx - 1 + graphic_size : _graphicIdx - 1;
                switch_graphics_library(graphics);
            } else if (act_event == Arcade::RESTART_GAME)
                game->restart();

            if (game->isGameOver())
                game->restart();
        }
    }

    void Core::run() {
        if (_gameIdx < 0 || _graphicIdx < 0 ||
            _gameIdx >= (int)_gamesNames.size() ||
            _graphicIdx >= (int)_graphicsNames.size())
            handle_error("Invalid library indexes");

        const std::string &gameKey = _gamesNames.at(_gameIdx);
        const std::string &graphicKey = _graphicsNames.at(_graphicIdx);

        for (const std::string &gameName : _gamesNames)
            std::cout << "Game: " << gameName << std::endl;
        for (const std::string &graphicName : _graphicsNames)
            std::cout << "Graphic: " << graphicName << std::endl;

        if (_gameLoaders.find(gameKey) == _gameLoaders.end())
            handle_error(("Game " + gameKey + " instance not found or null").c_str());
        if (_graphicLoaders.find(graphicKey) == _graphicLoaders.end())
            handle_error(("Graphic " + graphicKey + " instance not found or null").c_str());

        std::unique_ptr<IGame> game(
            _gameLoaders.at(gameKey).getInstance());
        std::unique_ptr<IGraphics> graphics(
            _graphicLoaders.at(graphicKey).getInstance());

        while (true) {
    
            if (!game) { handle_error("Game instance is null"); }
            if (!graphics) { handle_error("Graphics instance is null"); }

            std::vector<Arcade::Event> event = graphics->handleInput();
            handle_higher_order_events(event, game, graphics);
    
            if (!event.empty() && event.front().key == Arcade::QUIT)
                break;
    
            game->update(event);
    
            std::vector<std::pair<std::string, int>> highScores = {{"lol", 3}};
    
            graphics->clear();
            graphics->render(
                game->getMap(),
                game->getScore(),
                game->getName(),
                highScores
            );
        }
    }
} // namespace Arcade
