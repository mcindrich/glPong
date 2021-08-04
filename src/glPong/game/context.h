#ifndef glPong_GAME_CONTEXT_H
#define glPong_GAME_CONTEXT_H

#include <glPong/game/menu.h>
#include <glPong/game/config.h>
#include <glPong/game/mode.h>
#include <glPong/paddle.h>
#include <glPong/ball.h>

#define GAME_MODES_MAX_COUNT 5

typedef struct GameContext GameContext;

struct GameContext
{
    // game configuration values
    GameConfig cfg;
    // main menu
    GameMenu menu;
    // game state
    GameState state;
    // game modes
    GameMode *modes[GAME_MODES_MAX_COUNT];
    unsigned int modes_count;
    // chosen game mode - currently playing game mode
    GameMode *currentMode;
    // pointer to the GLFW main window
    GLFWwindow *winPtr;
};

// GameContext
void GameContextInit(GameContext *gc, GLFWwindow *winPtr);
int GameContextLoadConfig(GameContext *gc);
int GameContextLoadModes(GameContext *gc);
void GameContextLoadMenu(GameContext *gc);
void GameContextGameOver(GameContext *gc);
void GameContextDelete(GameContext *gc);

#endif // glPong_GAME_CONTEXT_H