/*
** EPITECH PROJECT, 2025
** Arcade
** File description:
** Event.hpp
*/

#pragma once

namespace Arcade {
enum KeyType {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPACE,
    ENTER,
    ESCAPE,
    NEXT_GAME,
    PREV_GAME,
    NEXT_GRAPHICS,
    PREV_GRAPHICS,
    RESTART_GAME,
    QUIT,
    MOUSE_CLICK,
    TEXT_INPUT,
    BACKSPACE,
    UNKNOWN
};

enum MouseButton {
    LEFT_CLICK,
    RIGHT_CLICK,
    MIDDLE_CLICK,
    NONE
};

struct Event {
        KeyType key;
        MouseButton mouseButton;
        int mouseX;
        int mouseY;
        char inputChar;

        Event(KeyType k) : key(k), mouseButton(MouseButton::NONE), mouseX(0), mouseY(0), inputChar(0)
        {
        }

        Event(MouseButton button, int x, int y)
            : key(KeyType::MOUSE_CLICK), mouseButton(button), mouseX(x), mouseY(y), inputChar(0)
        {
        }

        Event(KeyType k, char c) : key(k), mouseButton(MouseButton::NONE), mouseX(0), mouseY(0), inputChar(c)
        {
        }
};
}// namespace Arcade
