#include "glPong/log.h"
#include <cglm/vec2.h>
#include <glPong/game/config.h>
#include <glPong/game/menu.h>
#include <glPong/game/mode.h>
#include <glPong/game/state.h>
#include <glPong/paddle.h>
#include <glPong/game/context.h>
#include <cJSON.h>
#include <assert.h>

#define CONFIG_PATH "data/config.json"
#define LOAD_GAME_MODE(ctx, modeName)                                                                                  \
    do                                                                                                                 \
    {                                                                                                                  \
        extern GameMode modeName;                                                                                      \
        if ((ctx)->modes_count < GAME_MODES_MAX_COUNT)                                                                 \
        {                                                                                                              \
            (ctx)->modes[(ctx)->modes_count] = &modeName;                                                              \
            (ctx)->modes_count++;                                                                                      \
        }                                                                                                              \
    } while (0)

static void setDefaults(GameContext *gc);

void GameContextInit(GameContext *gc, GLFWwindow *winPtr)
{
    GameMenuInit(&gc->menu);
    GameConfigInit(&gc->cfg);
    gc->winPtr = winPtr;
    gc->state = GameStateMenu;

    // set game modes to 0
    gc->modes_count = 0;
    for (int i = 0; i < GAME_MODES_MAX_COUNT; i++)
    {
        gc->modes[i] = NULL;
    }
    gc->currentMode = NULL;
}

int GameContextLoadConfig(GameContext *gc)
{
    return GameConfigLoad(&gc->cfg, CONFIG_PATH);
}

int GameContextLoadModes(GameContext *gc)
{
    int err = 0;
    // load all wanted game modes
    LOAD_GAME_MODE(gc, classicGameMode);
    LOAD_GAME_MODE(gc, speedPlusPlusGameMode);

    // for now use this to assert all game modes above are loaded
    assert(gc->modes_count == 2);

    // after loading all game modes -> init them all and load their resources
    for (int i = 0; i < gc->modes_count; i++)
    {
        GameMode *mode = gc->modes[i];
        GameModeInit(mode, gc->winPtr, &gc->cfg);
        err = GameModeLoadResources(mode);
        if (err)
        {
            LogError("unable to load resources for game mode '%s'", mode->name);
            return err;
        }
    }

    // set to the first game mode loaded initially
    gc->currentMode = gc->modes[0];

    return err;
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
}

void GameContextDelete(GameContext *gc)
{
    GameMenuDelete(&gc->menu);
}

static void setDefaults(GameContext *gc)
{
    // load defaults first
    // DrawableSetSpeed(&gc->lPaddle.draw, 0.03);
    // DrawableSetSpeed(&gc->rPaddle.draw, 0.03);
    // DrawableSetSpeed(&gc->ball.draw, 0.01);
    // DrawableSetRectSize(&gc->lPaddle.draw, (vec2){20, 70});
    // DrawableSetRectSize(&gc->rPaddle.draw, (vec2){20, 70});
    // DrawableSetRectSize(&gc->ball.draw, (vec2){20, 20});
}
