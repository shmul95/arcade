/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** main
*/

#include "Arcade.hpp"

class IGraphics;

int main(int ac, char *av[])
{
    if (ac != 2)
        handle_error("wrong number of args");

    Arcade::Core core(av[1]);
    core.run();
    return 0;
}
