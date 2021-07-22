#include "glPong/game/state.h"
#include <glPong/game/context.h>

void GameContextInit(struct GameContext *gc, GLFWwindow *winPtr)
{
    // load paddles and a ball
    gc->lPaddle = PaddleNew();
    gc->rPaddle = PaddleNew();
    gc->ball = BallNew();

    gc->winPtr = winPtr;
    gc->state = GameStatePlaying;
    GameMenuInit(&gc->menu);

    DrawableSetSpeed(gc->lPaddle->draw, 0.03);
    DrawableSetSpeed(gc->rPaddle->draw, 0.03);
    DrawableSetSpeed(gc->ball->draw, 0.03);
    // DrawableSetRectSize(gc->lPaddle->draw, (vec2){0.05, 0.2});
    // DrawableSetRectSize(gc->rPaddle->draw, (vec2){0.05, 0.2});
    DrawableSetRectSize(gc->lPaddle->draw, (vec2){20, 70});
    DrawableSetRectSize(gc->rPaddle->draw, (vec2){20, 70});
    DrawableSetRectSize(gc->ball->draw, (vec2){0.08, 0.08});
}

void GameContextDelete(struct GameContext *gc)
{
    PaddleDelete(gc->lPaddle);
    free(gc->lPaddle);
    PaddleDelete(gc->rPaddle);
    free(gc->rPaddle);
    BallDelete(gc->ball);
    free(gc->ball);
    GameMenuDelete(&gc->menu);
}