#include <glPong/game/menu.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

// nuklear
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#include <nuklear.h>
#include <glPong/nuklear_glfw3.h>

void GameMenuInit(struct GameMenu *menu)
{
    menu->glfw = NULL;
    menu->bg = NULL;
    menu->ctx = NULL;
}

void GameMenuLoad(struct GameMenu *menu, GLFWwindow *win)
{
    struct nk_font_atlas *atlas;
    // allocate needed structs
    menu->glfw = malloc(sizeof(struct nk_glfw));
    menu->bg = malloc(sizeof(struct nk_colorf));

    menu->ctx = nk_glfw3_init(menu->glfw, win, NK_GLFW3_INSTALL_CALLBACKS);
    nk_glfw3_font_stash_begin(menu->glfw, &atlas);
    nk_glfw3_font_stash_end(menu->glfw);
    *menu->bg = (struct nk_colorf){0};
}

void GameMenuSetup(struct GameMenu *menu, enum GameState *state, unsigned int w, unsigned int h)
{
    struct nk_context *ctx = menu->ctx;
    struct nk_glfw *glfw = menu->glfw;
    struct nk_colorf *bg = menu->bg;

    nk_glfw3_new_frame(glfw);
    struct nk_style *s = &ctx->style;
    nk_style_push_color(ctx, &s->window.background, nk_rgba(0, 0, 0, 1));
    nk_style_push_style_item(ctx, &s->window.fixed_background, nk_style_item_color(nk_rgba(0, 0, 0, 1)));

    /* Config GUI */
    if (nk_begin(ctx, "Main Menu", nk_rect((float)w / 2 - 100, (float)h / 2 - 100, 200, 200),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
    {
        nk_layout_row_static(ctx, 30, 200, 1);
        nk_label(ctx, "glPong", NK_TEXT_CENTERED);

        nk_layout_row_dynamic(ctx, 20, 1);
        if (nk_button_label(ctx, "New Game"))
        {
            *state = GameStatePlaying;
        }
        if (nk_button_label(ctx, "Exit"))
        {
        }
    }
    nk_end(ctx);
    nk_style_pop_color(ctx);
    nk_style_pop_style_item(ctx);
}

void GameMenuRender(struct GameMenu *menu)
{
    nk_glfw3_render(menu->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void GameMenuDelete(struct GameMenu *menu)
{
    if (menu->glfw)
    {
        nk_glfw3_shutdown(menu->glfw);
    }
    free(menu->glfw);
    free(menu->bg);
}