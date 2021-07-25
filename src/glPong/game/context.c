#include "cglm/vec2.h"
#include "glPong/game/menu.h"
#include "glPong/game/state.h"
#include "glPong/paddle.h"
#include <glPong/game/context.h>
#include <cJSON.h>

static char *readFile(const char *path);
static void setDefaults(GameContext *gc);

void GameContextInit(GameContext *gc, GLFWwindow *winPtr)
{
    // load paddles and a ball
    char *cfgData = NULL;

    gc->lPaddle = PaddleNew();
    gc->rPaddle = PaddleNew();
    gc->ball = BallNew();

    gc->winPtr = winPtr;
    gc->state = GameStatePlaying;

    setDefaults(gc);

    // load json config and init data from it
    cfgData = readFile("data/config.json");
    if (cfgData)
    {
        cJSON *root = cJSON_Parse(cfgData);
        cJSON *paddle = NULL;
        cJSON *ball = NULL;

        paddle = cJSON_GetObjectItem(root, "paddle");
        ball = cJSON_GetObjectItem(root, "ball");
        if (paddle)
        {
            cJSON *paddleW = cJSON_GetObjectItem(paddle, "width");
            cJSON *paddleH = cJSON_GetObjectItem(paddle, "height");
            cJSON *paddleSpeed = cJSON_GetObjectItem(paddle, "speed");

            if (cJSON_IsNumber(paddleW) && cJSON_IsNumber(paddleH))
            {
                DrawableSetRectSize(gc->lPaddle->draw, (vec2){paddleW->valuedouble, paddleH->valuedouble});
                DrawableSetRectSize(gc->rPaddle->draw, (vec2){paddleW->valuedouble, paddleH->valuedouble});
            }

            if (cJSON_IsNumber(paddleSpeed))
            {
                DrawableSetSpeed(gc->lPaddle->draw, paddleSpeed->valuedouble);
                DrawableSetSpeed(gc->rPaddle->draw, paddleSpeed->valuedouble);
            }
        }
        if (ball)
        {
            cJSON *ballW = cJSON_GetObjectItem(ball, "width");
            cJSON *ballH = cJSON_GetObjectItem(ball, "height");
            cJSON *ballSpeed = cJSON_GetObjectItem(ball, "speed");

            if (cJSON_IsNumber(ballW) && cJSON_IsNumber(ballH))
            {
                DrawableSetRectSize(gc->ball->draw, (vec2){ballW->valuedouble, ballH->valuedouble});
            }

            if (cJSON_IsNumber(ballSpeed))
            {
                DrawableSetSpeed(gc->ball->draw, ballSpeed->valuedouble);
            }
        }
        cJSON_Delete(root);
        free(cfgData);
    }
    GameMenuInit(&gc->menu);
}

void GameContextLoadMenu(GameContext *gc)
{
    GameMenuLoad(&gc->menu, gc->winPtr);
    gc->state = GameStateMenu;
}

void GameContextGameOver(GameContext *gc)
{
    GameMenuDelete(&gc->menu);
    GameMenuLoad(&gc->menu, gc->winPtr);
    setDefaults(gc);
    DrawableSetDefaults(gc->ball->draw);
    DrawableSetDefaults(gc->lPaddle->draw);
    DrawableSetDefaults(gc->rPaddle->draw);
}

void GameContextDelete(GameContext *gc)
{
    PaddleDelete(gc->lPaddle);
    free(gc->lPaddle);
    PaddleDelete(gc->rPaddle);
    free(gc->rPaddle);
    BallDelete(gc->ball);
    free(gc->ball);
    GameMenuDelete(&gc->menu);
}

static char *readFile(const char *path)
{
    char *buffer = 0;
    long length;
    FILE *f = fopen(path, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(sizeof(char) * (length + 1));
        if (buffer)
        {
            fread(buffer, sizeof(char), length, f);
        }
        buffer[length] = 0;
        fclose(f);
    }

    return buffer;
}

static void setDefaults(GameContext *gc)
{
    // load defaults first
    DrawableSetSpeed(gc->lPaddle->draw, 0.03);
    DrawableSetSpeed(gc->rPaddle->draw, 0.03);
    DrawableSetSpeed(gc->ball->draw, 0.01);
    DrawableSetRectSize(gc->lPaddle->draw, (vec2){20, 70});
    DrawableSetRectSize(gc->rPaddle->draw, (vec2){20, 70});
    DrawableSetRectSize(gc->ball->draw, (vec2){20, 20});
}