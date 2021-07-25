#ifndef glPong_GAME_CONTEXT_H
#define glPong_GAME_CONTEXT_H

#include <glPong/game/menu.h>
#include <glPong/paddle.h>
#include <glPong/ball.h>

typedef struct GameContext GameContext;

struct GameContext
{
    Paddle *lPaddle;
    Paddle *rPaddle;
    Ball *ball;
    GameMenu menu;
    GameState state;
    GLFWwindow *winPtr;
};

// GameContext
void GameContextInit(GameContext *gc, GLFWwindow *winPtr);
void GameContextLoadMenu(GameContext *gc);
void GameContextGameOver(GameContext *gc);
void GameContextDelete(GameContext *gc);

#endif // glPong_GAME_CONTEXT_H