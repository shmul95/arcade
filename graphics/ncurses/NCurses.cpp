/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** NCurses
*/

#include "NCurses.hpp"
#include "IGraphics.hpp"

namespace Arcade
{
    static std::unordered_map<Color, int> init_colors(void)
    {
        start_color();

        init_pair(0, COLOR_BLACK, COLOR_BLACK);
        init_pair(1, COLOR_WHITE, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);
        init_pair(3, COLOR_GREEN, COLOR_BLACK);
        init_pair(4, COLOR_BLUE, COLOR_BLACK);
        init_pair(5, COLOR_YELLOW, COLOR_BLACK);
        init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(7, COLOR_CYAN, COLOR_BLACK);
        init_pair(8, COLOR_BLACK, COLOR_BLACK);

        init_pair(10, COLOR_BLACK, COLOR_BLACK);
        init_pair(11, COLOR_BLACK, COLOR_WHITE);
        init_pair(12, COLOR_BLACK, COLOR_RED);
        init_pair(13, COLOR_BLACK, COLOR_GREEN);
        init_pair(14, COLOR_BLACK, COLOR_BLUE);
        init_pair(15, COLOR_BLACK, COLOR_YELLOW);
        init_pair(16, COLOR_BLACK, COLOR_MAGENTA);
        init_pair(17, COLOR_BLACK, COLOR_CYAN);
        init_pair(18, COLOR_BLACK, COLOR_BLACK);

        std::unordered_map<Color, int> colors = {
            {Color::DEFAULT, 0},
            {Color::WHITE, 1},
            {Color::RED, 2},
            {Color::GREEN, 3},
            {Color::BLUE, 4},
            {Color::YELLOW, 5},
            {Color::PURPLE, 6},
            {Color::ORANGE, 7},
            {Color::BLACK, 8},
        };
        return colors;
    }

    NCurses::NCurses()
    {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(0);
        timeout(100);

        _name = "ncurses";
        _colors = init_colors();
   }

    NCurses::~NCurses() { endwin(); }

    static Event getEvent(int ch)
    {
        switch (ch) {
            case KEY_UP:    return Event(KeyType::UP);
            case KEY_DOWN:  return Event(KeyType::DOWN);
            case KEY_LEFT:  return Event(KeyType::LEFT);
            case KEY_RIGHT: return Event(KeyType::RIGHT);
            case ' ':       return Event(KeyType::SPACE);
            case '\n':      return Event(KeyType::ENTER);
            case 27:        return Event(KeyType::ESCAPE);
            case 'n':       return Event(KeyType::NEXT_GAME);
            case 'p':       return Event(KeyType::PREV_GAME);
            case 'g':       return Event(KeyType::NEXT_GRAPHICS);
            case 'h':       return Event(KeyType::PREV_GRAPHICS);
            case 'r':       return Event(KeyType::RESTART_GAME);
            case 'q':       return Event(KeyType::QUIT);
            default:        return Event(KeyType::UNKNOWN);
        }
    }

    std::vector<Event> NCurses::handleInput()
    {
        std::vector<Event> events;

        events.push_back(getEvent(getch()));
        return events;
    }

    static void draw_cell(
        int y, int x, char first, char second,
        bool invertedColor, const Cell cell,
        const std::unordered_map<Color, int> colors
    ) {
        attron(COLOR_PAIR(
            colors.at(cell.color) +
            (invertedColor ? 10 : 0)
        ) | A_BOLD);
        mvaddch(y, x * 2, cell.c ? ' ' : first);
        mvaddch(y, x * 2 + 1, cell.c ? cell.c : second);
        attroff(COLOR_PAIR(
            colors.at(cell.color) +
            (invertedColor ? 10 : 0)
        ) | A_BOLD);
    }

    static void draw_cell_select(
        int y, int x, const Cell cell,
        const std::unordered_map<Color, int> colors
    ) {
        switch (cell.type)
        {
            case CellContentType::EMPTY:        return draw_cell(y, x, ' ', ' ', true, cell, colors);
            case CellContentType::BLOCK:        return draw_cell(y, x, ' ', ' ', true, cell, colors);
            case CellContentType::PLAYER:       return draw_cell(y, x, ' ', ' ', true, cell, colors);
            case CellContentType::ENEMY:        return draw_cell(y, x, ' ', ' ', true, cell, colors);
            case CellContentType::FOOD:         return draw_cell(y, x, '<', '>', false, cell, colors);
            case CellContentType::POWERUP:      return draw_cell(y, x, '/', '\\', false, cell, colors);
            case CellContentType::PROJECTILE:   return draw_cell(y, x, '+', '+', false, cell, colors);
            case CellContentType::OBSTACLE:     return draw_cell(y, x, ' ', ' ', true, cell, colors);
            default:                            return draw_cell(y, x, ' ', ' ', true, cell, colors);
        }
    }

    static void render_map(
        const std::vector<std::vector<Cell>> map,
        const std::unordered_map<Color, int> colors
    ) {
        for (size_t y = 0; y < map.size(); y++) {
            for (size_t x = 0; x < map[y].size(); x++) {
                draw_cell_select(y, x, map[y][x], colors);
            }
        }
    }

    static void draw_sidebar_box(
        int startY, int startX,
        int boxHeight, int boxWidth
    ) {
        for (int y = startY; y < startY + boxHeight; ++y) {
            for (int x = startX; x < startX + boxWidth; ++x) {
                if (y == startY || y == startY + boxHeight - 1) {
                    if (x == startX) {
                        mvaddch(y, x, (y == startY) ? ACS_ULCORNER : ACS_LLCORNER);
                    } else if (x == startX + boxWidth - 1) {
                        mvaddch(y, x, (y == startY) ? ACS_URCORNER : ACS_LRCORNER);
                    } else {
                        mvaddch(y, x, ACS_HLINE);
                    }
                } else if (x == startX || x == startX + boxWidth - 1) {
                    mvaddch(y, x, ACS_VLINE);
                } else {
                    mvaddch(y, x, ' ');
                }
            }
        }
    }

    static void fill_sidebar_box(
        int startY, int startX, int score,
        const std::string playerName,
        const std::vector<std::pair<std::string, int>> highScores
    ) {
        mvprintw(startY + 1, startX + 2, "Player: %s", playerName.c_str());
        mvprintw(startY + 2, startX + 2, "Score: %i", score);

        mvprintw(startY + 4, startX + 2, "High Scores:");
        int scoreY = startY + 5;
        for (size_t i = 0; i < highScores.size() && i < 10; ++i) {
            mvprintw(scoreY + i, startX + 2, "%s: %u", 
                    highScores[i].first.c_str(), highScores[i].second);
        }
    }

    static void render_sidebar(
        int rows, int cols, int score,
        const std::string playerName,
        const std::vector<std::pair<std::string, int>> highScores
    ) {
        int boxHeight = rows;
        int boxWidth = cols / 3;
        int startY = 0;
        int startX = cols - boxWidth - 1;

        draw_sidebar_box(startY, startX, boxHeight, boxWidth);
        fill_sidebar_box(startY, startX, score, playerName, highScores);
    }

    void NCurses::render(
        const std::vector<std::vector<Cell>> &map,
        int score, const std::string &playerName,
        const std::vector<std::pair<std::string, int>> &highScores
    ) {
        int rows;
        int cols;

        getmaxyx(stdscr, rows, cols);
        clear();
        render_map(map, _colors);
        render_sidebar(rows, cols, score, playerName, highScores);
        refresh();
    }

    extern "C" IGraphics* createGraphics() { return new NCurses(); }
    extern "C" void destroyGraphics(IGraphics* graphics) { delete graphics; }
} // namespace Arcade
