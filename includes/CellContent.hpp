/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** CellContent.hpp
*/

#pragma once

namespace Arcade {
    enum CellContentType {
        EMPTY,
        BLOCK,
        PLAYER,
        ENEMY,
        FOOD,
        POWERUP,
        PROJECTILE,
        OBSTACLE,
    };
    
    enum Color {
        DEFAULT,
        WHITE,
        RED,
        GREEN,
        BLUE,
        YELLOW,
        PURPLE,
        ORANGE,
        BLACK,
    };
    
    struct Cell {
            int x;
            int y;
            char c;
            Color color;
            CellContentType type;
    };
}// namespace Arcade
