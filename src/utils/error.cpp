/*
** EPITECH PROJECT, 2025
** arcade
** File description:
** error
*/

#include "Arcade.hpp"

void handle_error(const char *msg)
{
    perror(msg);
    exit(84);
}
