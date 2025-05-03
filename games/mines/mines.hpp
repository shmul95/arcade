/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** mines
*/

#pragma once

#include "../../includes/IGame.hpp"
#include <vector>
#include <string>

namespace Arcade {

    class Mines : public IGame {
        public:
            Mines();
            ~Mines() override = default;
            void update(const std::vector<Event>& events) override;
            std::vector<std::vector<Cell>> getMap() const override;
            std::string getName() const override;
            int getScore() const override;
            bool isGameOver() const override;
            void close() override;

            MenuSelection getSelection() const override;
            void setAvailableLibraries(const std::vector<std::string>& gameLibs,
                const std::vector<std::string>& graphicsLibs) override;
            bool wantsTextInput() const override;
            void setPlayerName(const std::string &name) override;

        private:

            static const int MAP_W = 20;
            static const int MAP_H = 20;
            static const int MIN_MINE_COUNT = 30;
            static const int MAX_MINE_COUNT = 50;

            std::vector<std::vector<bool>> _mines;
            std::vector<std::vector<bool>> _revealed;
            std::vector<std::vector<bool>> _flagged;

            int _cursorX;
            int _cursorY;
            bool _gameOver;
            bool _won;
            bool _first_click;
            int _score;

            void generate_mines(int safeX, int safeY);
            int count_mines(int x, int y) const;
            void reveal_cell(int x, int y);
            void reveal_mines(int x, int y);
            bool check_win() const;
            void restart();
    };
}
