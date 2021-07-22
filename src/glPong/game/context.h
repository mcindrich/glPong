#ifndef glPong_GAME_CONTEXT_H
#define glPong_GAME_CONTEXT_H

#include <glPong/game/menu.h>
#include <glPong/paddle.h>
#include <glPong/ball.h>

struct GameContext
{
    struct Paddle *lPaddle;
    struct Paddle *rPaddle;
    struct Ball *ball;
    struct GameMenu menu;
    enum GameState state;
    GLFWwindow *winPtr;
};

// GameContext
void GameContextInit(struct GameContext *gc, GLFWwindow *winPtr);
void GameContextDelete(struct GameContext *gc);

#endif // glPong_GAME_CONTEXT_H