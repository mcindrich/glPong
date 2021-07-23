#ifndef glPong_GAME_MENU_H
#define glPong_GAME_MENU_H

#include <glPong/game/state.h>

typedef struct GLFWwindow GLFWwindow;

struct GameMenu
{
    struct nk_glfw *glfw;
    struct nk_context *ctx;
    struct nk_colorf *bg;
    int initialized;
};

void GameMenuInit(struct GameMenu *menu);
void GameMenuLoad(struct GameMenu *menu, GLFWwindow *win);
void GameMenuSetup(struct GameMenu *menu, enum GameState *state, unsigned int w, unsigned int h);
void GameMenuRender(struct GameMenu *menu);
void GameMenuDelete(struct GameMenu *menu);

#endif // glPong_GAME_MENU_H