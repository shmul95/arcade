/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** mines
*/

#include "mines.hpp"
#include <cstdlib>
#include <ctime>
#include <queue>

namespace Arcade {

    Mines::Mines()
        : _cursorX(0), _cursorY(0),
          _gameOver(false), _won(false), _first_click(true), _score(0)
    {
        std::srand(std::time(nullptr));
        _mines = std::vector<std::vector<bool>>(MAP_H, std::vector<bool>(MAP_W, false));
        _revealed = std::vector<std::vector<bool>>(MAP_H, std::vector<bool>(MAP_W, false));
        _flagged = std::vector<std::vector<bool>>(MAP_H, std::vector<bool>(MAP_W, false));
    }

    void Mines::restart()
    {
        _mines.assign(MAP_H, std::vector<bool>(MAP_W, false));
        _revealed.assign(MAP_H, std::vector<bool>(MAP_W, false));
        _flagged.assign(MAP_H, std::vector<bool>(MAP_W, false));
        _cursorX = 0;
        _cursorY = 0;
        _gameOver = false;
        _won = false;
        _first_click = true;
        _score = 0;
    }

    void Mines::generate_mines(int safeX, int safeY)
    {
        int placed = 0;
        int MINE_COUNT = MIN_MINE_COUNT + std::rand() % (MAX_MINE_COUNT - MIN_MINE_COUNT + 1);

        while (placed < MINE_COUNT) {
            int x = std::rand() % MAP_W;
            int y = std::rand() % MAP_H;
            if (_mines[y][x])
                continue;
            if (std::abs(x - safeX) <= 1 && std::abs(y - safeY) <= 1)
                continue;
            _mines[y][x] = true;
            placed++;
        }
    }

    int Mines::count_mines(int x, int y) const
    {
        int count = 0;
        for (int dy = -1; dy <= 1; dy++) {
            for (int dx = -1; dx <= 1; dx++) {
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < MAP_W && ny >= 0 && ny < MAP_H) {
                    if (_mines[ny][nx])
                        count++;
                }
            }
        }
        return count;
    }

    void Mines::reveal_cell(int x, int y) {
        if (x < 0 || x >= MAP_W || y < 0 || y >= MAP_H)
            return;
        if (x == 0 || x == MAP_W - 1 || y == 0 || y == MAP_H - 1)
            return;
        if (_revealed[y][x] || _flagged[y][x])
            return;
    
        _revealed[y][x] = true;
        _score += 1;
    
        if (_mines[y][x]) {
            _gameOver = true;
            for (int i = 0; i < MAP_H; i++) {
                for (int j = 0; j < MAP_W; j++) {
                    if (_mines[i][j])
                        _revealed[i][j] = true;
                }
            }
            return;
        }
        if (count_mines(x, y) == 0) {
            reveal_mines(x, y);
        }
    }

    void Mines::reveal_mines(int x, int y)
    {
        std::queue<std::pair<int, int>> q;
        q.push({x, y});
        _revealed[y][x] = true;
    
        while (!q.empty()) {
            auto [cx, cy] = q.front();
            q.pop();
    
            for (int dy = -1; dy <= 1; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int nx = cx + dx;
                    int ny = cy + dy;

                    if (nx >= 0 && nx < MAP_W && ny >= 0 && ny < MAP_H) {
                        if (!_revealed[ny][nx] && !_flagged[ny][nx]) {
                            _revealed[ny][nx] = true;
                            _score += 1;
                            if (count_mines(nx, ny) == 0) {
                                q.push({nx, ny});
                            }
                        }
                    }
                }
            }
        }
    }

    bool Mines::check_win() const
    {
        for (int y = 0; y < MAP_H; y++) {
            for (int x = 0; x < MAP_W; x++) {
                if (!_mines[y][x] && !_revealed[y][x])
                    return false;
            }
        }
        return true;
    }

    void Mines::update(const std::vector<Event>& events)
    {
        if (_gameOver)
            return;

        for (const auto &event : events) {
            if (event.key == UP) {
                if (_cursorY > 0) _cursorY--;
            } else if (event.key == DOWN) {
                if (_cursorY < MAP_H - 1) _cursorY++;
            } else if (event.key == LEFT) {
                if (_cursorX > 0) _cursorX--;
            } else if (event.key == RIGHT) {
                if (_cursorX < MAP_W - 1) _cursorX++;
            }
            else if (event.key == SPACE) {
                if (_first_click) {
                    generate_mines(_cursorX, _cursorY);
                    _first_click = false;
                }
                reveal_cell(_cursorX, _cursorY);
                if (check_win()) {
                    _gameOver = true;
                    _won = true;
                }
            }
            else if (event.key == ENTER) {
                if (_flagged[_cursorY][_cursorX]) {
                    _flagged[_cursorY][_cursorX] = false;
                } else {
                    _flagged[_cursorY][_cursorX] = true;
                }
            }
            else if (event.key == NEXT_GAME)
                _gameOver = true;
            else if (event.key == PREV_GAME)
                _gameOver = true;
            else if (event.key == RESTART_GAME)
                restart();
            else if (event.key == ESCAPE)
                _gameOver = true;
        }
    }

    std::vector<std::vector<Cell>> Mines::getMap() const
    {
        std::vector<std::vector<Cell>> map(MAP_H, std::vector<Cell>(MAP_W));
        for (int y = 0; y < MAP_H; y++) {
            for (int x = 0; x < MAP_W; x++) {
                Cell cell;
                cell.x = x;
                cell.y = y;
                cell.c = ' ';
                cell.color = DEFAULT;
                cell.type = BLOCK;
    
                if (x == 0 || x == MAP_W - 1 || y == 0 || y == MAP_H - 1) {
                    cell.color = PURPLE;
                    cell.type = BLOCK;
                } else if (_revealed[y][x]) {
                    if (_mines[y][x]) {
                        cell.c = '*';
                        cell.color = RED;
                        cell.type = BLOCK;
                    } else {
                        int adj = count_mines(x, y);
                        cell.c = (adj >= 0) ? ('0' + adj) : ' ';
                        cell.color = WHITE;
                        cell.type = BLOCK;
                    }
                } else {
                    if (_flagged[y][x]) {
                        cell.c = 'F';
                        cell.color = YELLOW;
                        cell.type = BLOCK;
                    } else {
                        cell.color = BLACK;
                        cell.type = BLOCK;
                    }
                }
                if (x == _cursorX && y == _cursorY) {
                    cell.color = ORANGE;
                }
                map[y][x] = cell;
            }
        }
        return map;
    }

    std::string Mines::getName() const
    {
        return "Minesweeper";
    }

    int Mines::getScore() const
    {
        return _score;
    }

    bool Mines::isGameOver() const
    {
        return _gameOver;
    }

    void Mines::close() {}

    MenuSelection Mines::getSelection() const {
        return {false, -1, -1};
    }

    void Mines::setAvailableLibraries([[maybe_unused]] const std::vector<std::string>& gameLibs,
                                        [[maybe_unused]] const std::vector<std::string>& graphicsLibs) {}

    bool Mines::wantsTextInput() const {
        return false;
    }

    void Mines::setPlayerName([[maybe_unused]] const std::string &name) {}

    extern "C" IGame* createGame()
    {
        return new Mines();
    }

    extern "C" void destroyGame(IGame* game) {
        delete game;
    }
}
