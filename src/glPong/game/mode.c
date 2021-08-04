#include "glPong/ball.h"
#include "glPong/paddle.h"
#include <glPong/game/mode.h>
#include <glPong/log.h>

void GameModeInit(GameMode *mode, GLFWwindow *win, GameConfig *cfg)
{
    // initialize the paddles and the ball
    PaddleInit(&mode->lPaddle);
    PaddleInit(&mode->rPaddle);
    BallInit(&mode->ball);

    mode->win = win;

    // set values from the config struct
    mode->ball.draw.speed = cfg->ballData.speed;
    mode->lPaddle.draw.speed = cfg->paddleData.speed;
    mode->rPaddle.draw.speed = cfg->paddleData.speed;

    glm_vec2_copy(cfg->ballData.rectSize, mode->ball.draw.rectSize);
    glm_vec2_copy(cfg->paddleData.rectSize, mode->lPaddle.draw.rectSize);
    glm_vec2_copy(cfg->paddleData.rectSize, mode->rPaddle.draw.rectSize);
}

int GameModeLoadResources(GameMode *mode)
{
    // load resources
    int err = 0;

    err = PaddleLoadResources(&mode->lPaddle);
    if (err)
    {
        LogError("unable to load left paddle resources...");
        goto error_out;
    }
    err = PaddleLoadResources(&mode->rPaddle);
    if (err)
    {
        LogError("unable to load right paddle resources...");
        goto error_out;
    }
    err = BallLoadResources(&mode->ball);
    if (err)
    {
        LogError("unable to load ball resources...");
        goto error_out;
    }

error_out:
    return err;
}

void GameModeDelete(GameMode *mode)
{
    if (mode->delete)
    {
        mode->delete (mode);
    }

    PaddleDelete(&mode->lPaddle);
    PaddleDelete(&mode->rPaddle);
    BallDelete(&mode->ball);
}