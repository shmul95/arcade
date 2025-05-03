/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** sfml
*/

#pragma once

#include "../../includes/IGraphics.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <SFML/Config.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <unordered_map>
#include <string>

namespace Arcade {
    class Arcade_SFML : public IGraphics {
        public:
            Arcade_SFML();
            ~Arcade_SFML();

            std::vector<Event> handleInput() override;
            void render(const std::vector<std::vector<Cell>> &map, int score,
                        const std::string &playerName, 
                        const std::vector<std::pair<std::string, int>> &highScores) override;
            void clear() override;
            std::string getName() const override;
            void close() override;
            
        private:
            void centerGameView(const std::vector<std::vector<Cell>>& map);
            sf::Vector2u _screenSize;
            sf::View _gameView;
            sf::RenderWindow _window;

            std::unordered_map<KeyType, sf::Keyboard::Key> _keyMapping;
            
            void drawCell(const Cell& cell);
            void drawText(const std::string &text, int x, int y, int size, sf::Color color);
            Event translateEvent(const sf::Event &sfEvent);
};
}
