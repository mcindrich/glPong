#include "glPong/direction.h"
#include <stdio.h>
#include <glPong/game/context.h>
#include <glPong/game/menu.h>
#include <glPong/log.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// window #def's
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700

// helpers
static void framebufferSizeCB(GLFWwindow *window, int w, int h);
static void processInput(GLFWwindow *window, struct GameContext *ctx);

int main()
{
    GLFWwindow *window = NULL;
    GLFWimage winIcon;
    GLint glewStatus = 0;
    struct GameContext ctx;

    int err = 0;
    int wWidth, wHeight;
    int nrChannelsIcon;

    // init window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // load icon first
    winIcon.pixels = stbi_load("deps/img/icon/icon.png", &winIcon.width, &winIcon.height, &nrChannelsIcon, 0);
    if (winIcon.pixels == NULL)
    {
        LogError("unable to load window icon");
        glfwTerminate();
        return -1;
    }
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "glPong", NULL, NULL);
    if (!window)
    {
        LogError("unable to create a GLFW window... exiting");
        glfwTerminate();
        return -1;
    }

    // window created -> make context current and initialize GLEW
    glfwMakeContextCurrent(window);

    glewStatus = glewInit();

    if (glewStatus != GLEW_OK)
    {
        LogError("unable to init GLEW... exiting");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCB);
    glfwSetWindowIcon(window, 1, &winIcon);

    // init game context
    GameContextInit(&ctx, window);

    // load resources
    err = PaddleLoadResources(ctx.lPaddle);
    if (err)
    {
        LogError("unable to load left paddle resources...");
        goto error_out;
    }
    err = PaddleLoadResources(ctx.rPaddle);
    if (err)
    {
        LogError("unable to load right paddle resources...");
        goto error_out;
    }
    err = BallLoadResources(ctx.ball);
    if (err)
    {
        LogError("unable to load ball resources...");
        goto error_out;
    }

    /* Intializes random number generator */
    srand(time(NULL));

    // all resources loaded normally -> start drawing the window and the game
    while (!glfwWindowShouldClose(window))
    {
        processInput(window, &ctx);
        glfwGetWindowSize(window, &wWidth, &wHeight);

        if (ctx.state == GameStateMenu)
        {
            GameMenuSetup(&ctx.menu, &ctx.state, wWidth, wHeight);
        }

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (ctx.state == GameStateMenu)
        {
            GameMenuRender(&ctx.menu);
        }
        else if (ctx.state == GameStatePlaying)
        {
            // if (ctx.ball->direction == DirectionNone)
            // {
            //     ctx.ball->direction = (rand() % 2 == 0) ? DirectionLeft : DirectionRight;
            //     const enum Direction dir = ctx.ball->direction;
            //     glm_vec2_copy((vec2){1, 0}, ctx.ball->dirUnitVec);
            // }
            // set resolution for all drawables
            glm_vec2((vec2){wWidth, wHeight}, ctx.lPaddle->draw->uResolution);
            glm_vec2((vec2){wWidth, wHeight}, ctx.rPaddle->draw->uResolution);
            glm_vec2((vec2){wWidth, wHeight}, ctx.ball->draw->uResolution);

            PaddleDraw(ctx.lPaddle, DirectionLeft);
            PaddleDraw(ctx.rPaddle, DirectionRight);
            BallDraw(ctx.ball);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

error_out:
    GameContextDelete(&ctx);
    glfwTerminate();
    if (winIcon.pixels)
    {
        free(winIcon.pixels);
    }
    return 0;
}

static void framebufferSizeCB(GLFWwindow *window, int w, int h)
{
    // change viewport to match new window size
    glViewport(0, 0, w, h);
}

static void processInput(GLFWwindow *window, struct GameContext *ctx)
{
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
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        PaddleMove(ctx->rPaddle, DirectionLeft);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        PaddleMove(ctx->rPaddle, DirectionRight);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        PaddleMove(ctx->lPaddle, DirectionUp);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        PaddleMove(ctx->lPaddle, DirectionDown);
    }
}