/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** sfml
*/

#include "sfml.hpp"

namespace Arcade {
    Arcade_SFML::Arcade_SFML() : _window(sf::VideoMode(1920, 1080), "Arcade SFML") {
        _keyMapping = {
            {UP, sf::Keyboard::Up},
            {DOWN, sf::Keyboard::Down},
            {LEFT, sf::Keyboard::Left},
            {RIGHT, sf::Keyboard::Right},
            {SPACE, sf::Keyboard::Space},
            {ENTER, sf::Keyboard::Enter},
            {ESCAPE, sf::Keyboard::Escape},
            {NEXT_GAME, sf::Keyboard::N},
            {PREV_GAME, sf::Keyboard::P},
            {NEXT_GRAPHICS, sf::Keyboard::G},
            {PREV_GRAPHICS, sf::Keyboard::H},
            {RESTART_GAME, sf::Keyboard::R},
            {QUIT, sf::Keyboard::Q},
        };
        _window.setFramerateLimit(11);
    }

    Arcade_SFML::~Arcade_SFML() {
        close();
    }

    std::vector<Event> Arcade_SFML::handleInput() {
        std::vector<Event> events;
        sf::Event event;

        while (_window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                events.push_back(Event(QUIT));
            else if (event.type == sf::Event::KeyPressed)
                events.push_back(translateEvent(event));
        }
        return events;
    }

    void Arcade_SFML::render(const std::vector<std::vector<Cell>>& map, int score,
                            const std::string& playerName, 
                            const std::vector<std::pair<std::string, int>>& highScores) {
        _window.clear();
                            
        for (const auto& row : map) {
            for (const auto& cell : row) {
                drawCell(cell);
            }
        }
    
        float uiX = map[0].size() * 21;
        float uiY = 10;

        drawText("Score: " + std::to_string(score), uiX, uiY, 30, sf::Color::White);
        uiY += 40;
        drawText("Controls:", uiX, uiY, 24, sf::Color::White);
        uiY += 30;
        drawText("Move: Arrow Keys", uiX, uiY, 20, sf::Color::Yellow);
        uiY += 25;
        drawText("Next Game: N", uiX, uiY, 20, sf::Color::Yellow);
        uiY += 25;
        drawText("Prev Game: P", uiX, uiY, 20, sf::Color::Yellow);
        uiY += 25;
        drawText("Next Graphics: G", uiX, uiY, 20, sf::Color::Yellow);
        uiY += 25;
        drawText("Prev Graphics: H", uiX, uiY, 20, sf::Color::Yellow);
        uiY += 25;
        drawText("Restart: R", uiX, uiY, 20, sf::Color::Yellow);
        uiY += 25;
        drawText("Quit: Q or ESC", uiX, uiY, 20, sf::Color::Yellow);

        _window.display();
    }

    void Arcade_SFML::clear() {
        _window.clear();
    }

    std::string Arcade_SFML::getName() const {
        return "SFML";
    }

    void Arcade_SFML::close() {
        if (_window.isOpen())
            _window.close();
    }

    void Arcade_SFML::drawCell(const Cell &cell) {
        sf::RectangleShape rect(sf::Vector2f(20, 20));
        
        std::unordered_map<Color, sf::Color> colorMap = {
            {DEFAULT, sf::Color::White},
            {WHITE,   sf::Color::White},
            {RED,     sf::Color::Red},
            {GREEN,   sf::Color::Green},
            {BLUE,    sf::Color::Blue},
            {YELLOW,  sf::Color::Yellow},
            {PURPLE,  sf::Color(128, 0, 128)},
            {ORANGE,  sf::Color(255, 165, 0)},
            {BLACK,   sf::Color::Black}
        };
        
        sf::Color fillColor = colorMap[cell.color];
        float posX = cell.x * 20.0f;
        float posY = cell.y * 20.0f;
    
        switch(cell.type) {
            case PLAYER: {
                sf::CircleShape shape(10.0f);
                shape.setFillColor(fillColor);
                shape.setPosition(posX, posY);
                _window.draw(shape);
                break;
            }

            case ENEMY: {
                sf::CircleShape shape(10.0f);
                shape.setFillColor(fillColor);
                shape.setPosition(posX, posY);
                _window.draw(shape);
                break;
            }

            case FOOD: {
                sf::CircleShape outerCircle(10.0f);
                outerCircle.setFillColor(fillColor);
                outerCircle.setPosition(posX, posY);
            
                sf::CircleShape innerCircle(8.0f);
                innerCircle.setFillColor(sf::Color::Black);
                innerCircle.setPosition(posX + 4, posY + 1);
            
                _window.draw(outerCircle);
                _window.draw(innerCircle);
                break;
            }
            
            case BLOCK: {
                sf::RectangleShape rect(sf::Vector2f(20, 20));
                rect.setFillColor(fillColor);
                rect.setPosition(posX, posY);
                _window.draw(rect);
                break;
            }
            
            case PROJECTILE: {
                sf::CircleShape shape(5.0f);
                shape.setFillColor(fillColor);
                shape.setPosition(posX + 5, posY + 5);
                _window.draw(shape);
                break;
            }
            
            case EMPTY: {
                sf::RectangleShape emptyRect(sf::Vector2f(20, 20));
                emptyRect.setFillColor(sf::Color::Transparent);
                emptyRect.setPosition(posX, posY);
                _window.draw(emptyRect);
                break;
            }
            
            case POWERUP: {
                sf::RectangleShape powerupRect(sf::Vector2f(20, 20));
                powerupRect.setFillColor(fillColor);
                powerupRect.setPosition(posX, posY);
                _window.draw(powerupRect);
                break;
            }

            case OBSTACLE: {
                sf::RectangleShape obstacleRect(sf::Vector2f(20, 20));
                obstacleRect.setFillColor(fillColor);
                obstacleRect.setPosition(posX, posY);
                _window.draw(obstacleRect);
                break;
            }

            case UNKNOWN: {
                break;
            }

            default: {
                sf::RectangleShape rect(sf::Vector2f(20, 20));
                rect.setFillColor(fillColor);
                rect.setPosition(posX, posY);
                _window.draw(rect);
            }
        }
        if (cell.c != ' ') {
            sf::Text text;
            sf::Font font;
            if (!font.loadFromFile("assets/fonts/Arial.ttf")) {
                return;
            }
            text.setFont(font);
            text.setString(std::string(1, cell.c));
            text.setCharacterSize(20);
            text.setFillColor(sf::Color::Black);
            text.setPosition(posX + 5, posY + 2);
            _window.draw(text);
        }
    }

    void Arcade_SFML::drawText(const std::string &text, int x, int y, int size, sf::Color color) {
        static sf::Font font;
        if (!font.loadFromFile("assets/fonts/Arial.ttf"))
            return;
        sf::Text sfText(text, font, size);
        sfText.setFillColor(color);
        sfText.setPosition(x, y);
        _window.draw(sfText);
    }

    Event Arcade_SFML::translateEvent(const sf::Event &sfEvent) {
        if (sfEvent.type == sf::Event::KeyPressed) {
            for (const auto &pair : _keyMapping) {
                if (sfEvent.key.code == pair.second)
                    return Event(pair.first);
            }
        } else if (sfEvent.type == sf::Event::MouseButtonPressed) {
            switch (sfEvent.mouseButton.button) {
                case sf::Mouse::Left:
                    return Event(LEFT_CLICK, sfEvent.mouseButton.x, sfEvent.mouseButton.y);
                case sf::Mouse::Right:
                    return Event(RIGHT_CLICK, sfEvent.mouseButton.x, sfEvent.mouseButton.y);
                case sf::Mouse::Middle:
                    return Event(MIDDLE_CLICK, sfEvent.mouseButton.x, sfEvent.mouseButton.y);
                default:
                    break;
            }
        }
        return Event(UNKNOWN);
    }
    extern "C" IGraphics *createGraphics() { return new Arcade_SFML(); }
    extern "C" void destroyGraphics(IGraphics *graphics) { delete graphics; }
}
