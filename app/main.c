#include <glPong/ball.h>
#include <glPong/direction.h>
#include <glPong/drawable.h>
#include <glPong/log.h>
#include <glPong/paddle.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glPong.h>

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

// window #def's
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

enum GameState
{
    GameStateMenu = 0,
    GameStateCountdown,
    GameStatePlaying,
    GameStateOver,
};

struct GameMenu
{
    struct nk_glfw glfw;
    struct nk_context *ctx;
    struct nk_colorf bg;
    enum GameState state;
};

struct GameContext
{
    struct Paddle *lPaddle;
    struct Paddle *rPaddle;
    struct Ball *ball;
    struct GameMenu menu;
    GLFWwindow *winPtr;
};

// GameContext
static void GameContextInit(struct GameContext *gc, GLFWwindow *winPtr);
static void GameContextDelete(struct GameContext *gc);

// GameMenu
static void GameMenuInit(struct GameMenu *menu, GLFWwindow *win);
static void GameMenuSetup(struct GameMenu *menu);
static void GameMenuRender(struct GameMenu *menu);
static void GameMenuDelete(struct GameMenu *menu);

// helpers
static void framebufferSizeCB(GLFWwindow *window, int w, int h);
static void processInput(GLFWwindow *window, struct GameContext *ctx);
static void modifyPaddle(struct GameContext *gc, struct Paddle *p);

int main()
{
    GLFWwindow *window = NULL;

    GLint glewStatus = 0;
    struct GameContext ctx;
    struct nk_context nkCtx;

    int err = 0;
    int wWidth, wHeight;

    // init window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "glPong", NULL, NULL);
    if (!window)
    {
        LogError("Unable to create a GLFW window... exiting");
        glfwTerminate();
        return -1;
    }

    // window created -> make context current and initialize GLEW
    glfwMakeContextCurrent(window);

    glewStatus = glewInit();

    if (glewStatus != GLEW_OK)
    {
        LogError("Unable to init GLEW... exiting");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCB);

    // init game context
    GameContextInit(&ctx, window);

    DrawableSetSpeed(ctx.lPaddle->draw, 0.03);
    DrawableSetSpeed(ctx.rPaddle->draw, 0.03);
    DrawableSetSpeed(ctx.ball->draw, 0.03);
    DrawableSetRectSize(ctx.lPaddle->draw, (vec2){0.05, 0.2});
    DrawableSetRectSize(ctx.rPaddle->draw, (vec2){0.05, 0.2});

    // load resources
    err = PaddleLoadResources(ctx.lPaddle);
    if (err)
    {
        LogError("Unable to load left paddle resources...");
        goto error_out;
    }
    err = PaddleLoadResources(ctx.rPaddle);
    if (err)
    {
        LogError("Unable to load right paddle resources...");
        goto error_out;
    }
    err = BallLoadResources(ctx.ball);
    if (err)
    {
        LogError("Unable to load ball resources...");
        goto error_out;
    }

    // all resources loaded normally -> start drawing the window and the game
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, &ctx);

        if (ctx.menu.state == GameStateMenu)
        {
            GameMenuSetup(&ctx.menu);
        }

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwGetWindowSize(window, &wWidth, &wHeight);

        if (ctx.menu.state == GameStateMenu)
        {
            GameMenuRender(&ctx.menu);
        }
        else
        {
            // set resolution for all drawables
            glm_vec2((vec2){wWidth, wHeight}, ctx.lPaddle->draw->uResolution);
            glm_vec2((vec2){wWidth, wHeight}, ctx.rPaddle->draw->uResolution);
            glm_vec2((vec2){wWidth, wHeight}, ctx.ball->draw->uResolution);

            PaddleDraw(ctx.lPaddle, 1);
            PaddleDraw(ctx.rPaddle, 0);
            BallDraw(ctx.ball);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

error_out:
    GameContextDelete(&ctx);

    glfwTerminate();
    return 0;
}

static void framebufferSizeCB(GLFWwindow *window, int w, int h)
{
    // change viewport to match new window size
    glViewport(0, 0, w, h);
}

static void processInput(GLFWwindow *window, struct GameContext *ctx)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        PaddleMove(ctx->rPaddle, DirectionUp);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        PaddleMove(ctx->rPaddle, DirectionDown);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        PaddleMove(ctx->lPaddle, DirectionUp);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        PaddleMove(ctx->lPaddle, DirectionDown);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        PaddleMove(ctx->rPaddle, DirectionLeft);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        PaddleMove(ctx->rPaddle, DirectionRight);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        PaddleMove(ctx->lPaddle, DirectionLeft);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        PaddleMove(ctx->lPaddle, DirectionRight);
    }
}

static void GameContextInit(struct GameContext *gc, GLFWwindow *winPtr)
{
    // load paddles and a ball
    gc->lPaddle = PaddleNew();
    gc->rPaddle = PaddleNew();
    gc->ball = BallNew();
    gc->winPtr = winPtr;
    GameMenuInit(&gc->menu, winPtr);
}

static void GameContextDelete(struct GameContext *gc)
{
    PaddleDelete(gc->lPaddle);
    PaddleDelete(gc->rPaddle);
    BallDelete(gc->ball);
    GameMenuDelete(&gc->menu);
}

static void GameMenuInit(struct GameMenu *menu, GLFWwindow *win)
{
    struct nk_font_atlas *atlas;
    menu->ctx = nk_glfw3_init(&menu->glfw, win, NK_GLFW3_INSTALL_CALLBACKS);
    nk_glfw3_font_stash_begin(&menu->glfw, &atlas);
    nk_glfw3_font_stash_end(&menu->glfw);
    menu->bg = (struct nk_colorf){0};
    menu->state = GameStateMenu;
}

static void GameMenuSetup(struct GameMenu *menu)
{
    struct nk_context *ctx = menu->ctx;
    struct nk_glfw *glfw = &menu->glfw;
    struct nk_colorf *bg = &menu->bg;

    nk_glfw3_new_frame(glfw);
    struct nk_style *s = &ctx->style;
    nk_style_push_color(ctx, &s->window.background, nk_rgba(0, 0, 0, 1));
    nk_style_push_style_item(ctx, &s->window.fixed_background, nk_style_item_color(nk_rgba(0, 0, 0, 1)));

    /* Config GUI */
    if (nk_begin(ctx, "Main Menu", nk_rect(WINDOW_WIDTH / 2 - 100, WINDOW_HEIGHT / 2 - 100, 200, 200),
                 NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR))
    {
        enum
        {
            EASY,
            HARD
        };
        static int op = EASY;
        static int property = 20;

        nk_layout_row_static(ctx, 30, 200, 1);
        nk_label(ctx, "glPong", NK_TEXT_CENTERED);
        // nk_layout_row_static(ctx, 30, 200, 2);
        nk_layout_row_dynamic(ctx, 20, 1);
        if (nk_button_label(ctx, "New Game"))
        {
            menu->state = GameStateCountdown;
        }
        if (nk_button_label(ctx, "Exit"))
        {
        }
    }
    nk_end(ctx);
    nk_style_pop_color(ctx);
    nk_style_pop_style_item(ctx);
}

static void GameMenuRender(struct GameMenu *menu)
{
    nk_glfw3_render(&menu->glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

static void GameMenuDelete(struct GameMenu *menu)
{
    nk_glfw3_shutdown(&menu->glfw);
}