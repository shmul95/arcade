
# Arcade Project Documentation

## Adding New Graphics Libraries

To implement a new graphical library compatible with the Arcade system, follow these steps:

1. **Create a new shared object (.so)** with the prefix `arcade_`, e.g., `arcade_mygraphics.so`.
2. Implement the `IGraphics` interface defined in `IGraphics.hpp`. Required methods:
   - `handleInput()`
   - `render()`
   - `clear()`
   - `getName()`
   - `close()`
1. Provide factory function with C linkage:
```cpp
extern "C" IGraphics* createGraphics();
```
4. Place your `.so` file into the `./lib/` directory.

## Adding New Game Libraries

1. **Create a new shared object (.so)** with the prefix `arcade_`, e.g., `arcade_mygame.so`.
2. Implement the `IGame` interface defined in `IGame.hpp`. Required methods:
   - `update()`
   - `getMap()`
   - `getScore()`
   - `getName()`
   - `isGameOver()`
   - `restart()`
   - `close()`
1. Provide factory function with C linkage:
```cpp
extern "C" IGame* createGame();
```
4. Place your `.so` file into the `./lib/` directory.

## Class Diagram and Explanation

```
+------------------------+      loads via     +----------------------+
|        Core            |------------------->|      DLLoader        |
|------------------------|                    |----------------------|
| - _gameLoaders         |                    | - handler            |
| - _graphicLoaders      |                    |                      |
| - _gamesNames          |                    | + getInstance        |
| - _graphicsNames       |                    +----------------------+
| - _gameIdx             |                           |
| - _graphicIdx          |     getInstance()         |
|                        |---------------------------+
| +run()                 |
+------------------------+
         |    |
         |    +-----------------------+
         |                            |
         V                            V
+-------------------+       +------------------------+
| IGame (instance)  |       | IGraphics (instance)   |
|-------------------|       |------------------------|
| +update(events)   |       | +handleInput()         |
| +getMap()         |       | +render(map, score,...)|
| +getScore()       |       | +clear()               |
| +isGameOver()     |       |                        |
+-------------------+       +------------------------+
         ▲                            ▲
         |                            |
         | Implements                 | Implements
         |                            |
+-------------------+       +------------------------+
|      Snake        |       |    SDL2 / SFML / ...   |
+-------------------+       +------------------------+
```

### Flow Summary:

1. **Core** loads libraries using `DLLoader`, storing them in `_gameLoaders` and `_graphicLoaders`.
2. Using `getInstance()` from `DLLoader`, it gets concrete instances of `IGame` and `IGraphics`.
3. In the game loop:
   - `graphics->handleInput()` is called to capture input.
   - The input is passed to `game->update(events)`.
   - Then, the game state is rendered via `graphics->render(...)`.

## Manual: How the System Works

1. At startup, `main.cpp` initializes `Core` with the selected graphical library.
2. `Core` loads all available `.so` files from `./lib/`, differentiates graphics from game libraries.
3. Libraries are instantiated using factory functions from `DLLoader`.
4. The main loop:
   - Handles user input
   - Updates game logic
   - Renders the updated state

Dynamic switching of games and graphics libraries is supported during runtime with predefined key mappings.

