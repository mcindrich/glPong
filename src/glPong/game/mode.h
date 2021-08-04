#ifndef glPong_GAME_MODE_H
#define glPong_GAME_MODE_H

#include <glPong/paddle.h>
#include <glPong/ball.h>
#include <glPong/game/state.h>
#include <glPong/game/config.h>

typedef struct GLFWwindow GLFWwindow;
typedef struct GameMode GameMode;

struct GameMode
{
    // game mode info - name and a description to be shown in the main menu
    const char *name;
    const char *desc;
    // paddles array
    Paddle *paddleArray;
    unsigned int paddleArrayN;
    // balls array
    Ball *ballArray;
    unsigned int ballArrayN;

    // TODO: remove
    Paddle lPaddle;
    Paddle rPaddle;
    Ball ball;
    GLFWwindow *win;
    GameState *state;

    // callbacks
    int (*init)(GameMode *mode, GLFWwindow *win);
    int (*processInput)(GameMode *mode);
    int (*draw)(GameMode *mode, GameState *state, int wWidth, int wHeight);
    int (*delete)(GameMode *mode);
};

void GameModeInit(GameMode *mode, GLFWwindow *win, GameConfig *cfg);
int GameModeLoadResources(GameMode *mode);
void GameModeDelete(GameMode *mode);

#endif // glPong_GAME_MODE_H