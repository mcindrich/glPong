#include "glPong/ball.h"
#include "glPong/direction.h"
#include "glPong/drawable.h"
#include "glPong/log.h"
#include "glPong/paddle.h"
#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glPong.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

struct GameContext
{
    struct Paddle *lPaddle;
    struct Paddle *rPaddle;
    struct Ball *ball;
};

static void GameContextInit(struct GameContext *gc);
static void GameContextDelete(struct GameContext *gc);

static void framebufferSizeCB(GLFWwindow *window, int w, int h);
static void processInput(GLFWwindow *window, struct GameContext *ctx);
static void modifyPaddle(struct GameContext *gc, struct Paddle *p);

int main()
{
    GLFWwindow *window = NULL;
    GLint glewStatus = 0;
    struct GameContext ctx;
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
    GameContextInit(&ctx);

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

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwGetWindowSize(window, &wWidth, &wHeight);

        // set resolution for all drawables
        glm_vec2((vec2){wWidth, wHeight}, ctx.lPaddle->draw->uResolution);
        glm_vec2((vec2){wWidth, wHeight}, ctx.rPaddle->draw->uResolution);
        glm_vec2((vec2){wWidth, wHeight}, ctx.ball->draw->uResolution);

        PaddleDraw(ctx.lPaddle, 1);
        PaddleDraw(ctx.rPaddle, 0);
        BallDraw(ctx.ball);

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

static void GameContextInit(struct GameContext *gc)
{
    // load paddles and a ball
    gc->lPaddle = PaddleNew();
    gc->rPaddle = PaddleNew();
    gc->ball = BallNew();
}

static void GameContextDelete(struct GameContext *gc)
{
    PaddleDelete(gc->lPaddle);
    PaddleDelete(gc->rPaddle);
    BallDelete(gc->ball);
}