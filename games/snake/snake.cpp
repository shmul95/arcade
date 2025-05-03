/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** snake
*/

#include "snake.hpp"
#include <cstdlib>
#include <ctime>
#include <algorithm>

namespace Arcade {

    Snake::Snake() : _score(0), _gameOver(false), _dir(RIGHT), _time_last_move(0) {
        std::srand(std::time(nullptr));
        restart();
    }

    void Snake::restart() {
        _snake.clear();
        int startX = MAP_W / 2;
        int startY = MAP_H / 2;

        for (int i = 0; i < 4; i++)
            _snake.push_back({startX - i, startY});
        _dir = RIGHT;
        _score = 0;
        _gameOver = false;
        place_food();
    }

    void Snake::place_food() {
        do {
            _food.first = 1 + rand() % (MAP_W - 2);
            _food.second = 1 + rand() % (MAP_H - 2);
        } while (std::find(_snake.begin(), _snake.end(), _food) != _snake.end());
    }

    void Snake::draw_borders(std::vector<std::vector<Cell>>& map) const {
        for (int x = 0; x < MAP_W; x++) {
            map[0][x] = {x, 0, 0, WHITE, BLOCK};
            map[MAP_H - 1][x] = {x, MAP_H - 1, 0, WHITE, BLOCK};
        }
        for (int y = 0; y < MAP_H; y++) {
            map[y][0] = {0, y, 0, WHITE, BLOCK};
            map[y][MAP_W - 1] = {MAP_W - 1, y, 0, WHITE, BLOCK};
        }
    }

    void Snake::move_snake() {
        auto head = _snake.front();

        switch (_dir) {
            case UP:    head.second--; break;
            case DOWN:  head.second++; break;
            case LEFT:  head.first--; break;
            case RIGHT: head.first++; break;
            default: break;
        }

        if (head.first <= 0 || head.first >= MAP_W - 1 ||
            head.second <= 0 || head.second >= MAP_H - 1) {
            _gameOver = true;
            return;
        }
        if (std::find(_snake.begin(), _snake.end(), head) != _snake.end()) {
            _gameOver = true;
            return;
        }
        _snake.insert(_snake.begin(), head);
        if (head == _food) {
            _score += 10;
            place_food();
        } else {
            _snake.pop_back();
        }
    }

    void Snake::update(const std::vector<Event>& events) {
        for (const auto& event : events) {
            if (event.key == ESCAPE) {
                _gameOver = true;
                return;
            }
            if (event.key == UP && _dir != DOWN)
                _dir = UP;
            else if (event.key == DOWN && _dir != UP)
                _dir = DOWN;
            else if (event.key == LEFT && _dir != RIGHT)
                _dir = LEFT;
            else if (event.key == RIGHT && _dir != LEFT)
                _dir = RIGHT;
            else if (event.key == SPACE)
                restart();
        }
        move_snake();
    }

    std::vector<std::vector<Cell>> Snake::getMap() const
    {
        std::vector<std::vector<Cell>> map(MAP_H,
            std::vector<Cell>(MAP_W, {0, 0, ' ', BLACK, EMPTY}));

        draw_borders(map);
        for (size_t i = 0; i < _snake.size(); i++) {
            auto& part = _snake[i];
            map[part.second][part.first] = {
                part.first,
                part.second,
                0,
                (i == 0) ? GREEN : GREEN,
                (i == 0) ? PLAYER : ENEMY
            };
        }
        map[_food.second][_food.first] = {
            _food.first,
            _food.second,
            0,
            YELLOW,
            FOOD
        };
        return map;
    }

    std::string Snake::getName() const {
        return "Snake";
    }

    int Snake::getScore() const {
        return _score;
    }

    bool Snake::isGameOver() const {
        return _gameOver;
    }

    void Snake::close() {}

    MenuSelection Snake::getSelection() const {
        return {false, -1, -1};
    }

    void Snake::setAvailableLibraries([[maybe_unused]] const std::vector<std::string>& gameLibs,
                                        [[maybe_unused]] const std::vector<std::string>& graphicsLibs) {
    }

    bool Snake::wantsTextInput() const {
        return false;
    }

    void Snake::setPlayerName([[maybe_unused]] const std::string &name) {
    }

    extern "C" IGame* createGame() {
        return new Snake();
    }

    extern "C" void destroyGame(IGame* game) {
        delete game;
    }
}
