/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** snake
*/

#pragma once
#include "../../includes/IGame.hpp"
#include "../../includes/Event.hpp"
#include <vector>
#include <utility>
#include <cstdlib>

namespace Arcade {
    class Snake : public IGame {
    private:
        const int MAP_W = 40;
        const int MAP_H = 30;
       
        int _score;
        bool _gameOver;
        KeyType _dir;
        std::vector<std::pair<int, int>> _snake;
        std::pair<int, int> _food;
        float _time_last_move;
        
        void place_food();
        void move_snake();
        void draw_borders(std::vector<std::vector<Cell>>& map) const;
        
    public:
        Snake();
        ~Snake() override = default;
        void update(const std::vector<Event>& events) override;
        std::vector<std::vector<Cell>> getMap() const override;
        std::string getName() const override;
        int getScore() const override;
        bool isGameOver() const override;
        void restart() override;
        void close() override;
        MenuSelection getSelection() const override;
        void setAvailableLibraries(const std::vector<std::string>& gameLibs,
                                     const std::vector<std::string>& graphicsLibs) override;
        bool wantsTextInput() const override;
        void setPlayerName(const std::string &name) override;
    };

    extern "C" IGame* createGame();
    extern "C" void destroyGame(IGame* game);
}

