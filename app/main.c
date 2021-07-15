#include "glPong/ball.h"
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

void GameContextInit(struct GameContext *gc);
void GameContextDelete(struct GameContext *gc);

static void framebufferSizeCB(GLFWwindow *window, int w, int h);
static void processInput(GLFWwindow *window, struct GameContext *ctx);

int main()
{
    GLFWwindow *window = NULL;
    GLint glewStatus = 0;
    struct GameContext ctx;
    int err = 0;

    // init window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

        // glClearColor(0.3f, 0.2f, 0.17f, 1.0f);
        // glClear(GL_COLOR_BUFFER_BIT);

        PaddleDraw(ctx.lPaddle);
        PaddleDraw(ctx.rPaddle);
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
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        // make player go up
    }
}

void GameContextInit(struct GameContext *gc)
{
    // load paddles and a ball
    gc->lPaddle = PaddleNew();
    gc->rPaddle = PaddleNew();
    gc->ball = BallNew();
}

void GameContextDelete(struct GameContext *gc)
{
    PaddleDelete(gc->lPaddle);
    PaddleDelete(gc->rPaddle);
    BallDelete(gc->ball);
}