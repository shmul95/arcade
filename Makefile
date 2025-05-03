##
## EPITECH PROJECT, 2025
## arcade
## File description:
## Makefile
##

.SILENCE: all core graphics game clean re

CXX = g++
CXXFLAGS = -I./includes -fPIC -fno-gnu-unique -g
DLFLAGS = -ldl
NCURSESFLAGS = -lncurses
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system
SDLFLAGS = -lSDL2 -lSDL2_ttf


NAME = arcade

SRC_DIR = src
GRAPHICS_DIR = graphics
GAME_DIR = games
LIB_DIR = lib

SRC = 	main.cpp \
		$(SRC_DIR)/Core.cpp \
		$(SRC_DIR)/utils/error.cpp

# ------------- GRAPHICS -------------

NCURSES_DIR = $(GRAPHICS_DIR)/ncurses
NCURSES_SRC = $(NCURSES_DIR)/NCurses.cpp
NCURSES_LIB = $(LIB_DIR)/arcade_ncurses.so

SDL_DIR = $(GRAPHICS_DIR)/sdl2
SDL_SRC = $(SDL_DIR)/sdl2.cpp
SDL_LIB = $(LIB_DIR)/arcade_sdl2.so

SFML_DIR = $(GRAPHICS_DIR)/sfml
SFML_SRC = $(SFML_DIR)/sfml.cpp
SFML_LIB = $(LIB_DIR)/arcade_sfml.so

# ------------- GAMES -------------

SNAKE_DIR = $(GAME_DIR)/snake
SNAKE_SRC = $(SNAKE_DIR)/snake.cpp
SNAKE_LIB = $(LIB_DIR)/arcade_snake.so

MINES_DIR = $(GAME_DIR)/mines
MINES_SRC = $(MINES_DIR)/mines.cpp
MINES_LIB = $(LIB_DIR)/arcade_minesweeper.so

all: core graphicals games

core: $(SRC)
	mkdir -p $(LIB_DIR)
	$(CXX) -o $(NAME) $(SRC) $(CXXFLAGS) $(DLFLAGS)

graphicals: $(NCURSES_SRC) $(SFML_SRC) $(SDL_SRC)
	mkdir -p $(LIB_DIR)
##	$(CXX) -shared -o $(NCURSES_LIB) $(NCURSES_SRC) $(CXXFLAGS) $(NCURSESFLAGS)
	$(CXX) -shared -o $(SFML_LIB) $(SFML_SRC) $(CXXFLAGS) $(SFMLFLAGS)
	$(CXX) -shared -o $(SDL_LIB) $(SDL_SRC) $(CXXFLAGS) $(SDLFLAGS)

games: $(SNAKE_SRC)
	mkdir -p $(LIB_DIR)
	$(CXX) -shared -o $(SNAKE_LIB) $(SNAKE_SRC) $(CXXFLAGS)
	$(CXX) -shared -o $(MINES_LIB) $(MINES_SRC) $(CXXFLAGS)

clean:
	rm -f $(NCURSES_LIB)

fclean: clean
	rm -f $(NAME) lib/*.so

re: fclean all

.PHONY: all core graphics game clean re

