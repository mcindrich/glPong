#include <glPong/ball.h>
#include <glPong/direction.h>
#include <glPong/game/state.h>
#include <glPong/game/context.h>
#include <glPong/game/menu.h>
#include <glPong/log.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <time.h>

// stb_image
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// window #def's - starting width and height
#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 700

// helpers
static void framebufferSizeCB(GLFWwindow *window, int w, int h);
static void processInput(GLFWwindow *window, GameContext *ctx);

int main()
{
    GLFWwindow *window = NULL;
    GLFWimage winIcon;
    GLint glewStatus = 0;
    GameContext ctx;

    int err = 0;
    int wWidth, wHeight;
    int nrChannelsIcon;

    // init window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

    err = GameContextLoadConfig(&ctx);
    if (err)
    {
        LogError("unable to load game configuration correctly...");
        goto error_out;
    }

    err = GameContextLoadModes(&ctx);
    if (err)
    {
        LogError("unable to load game modes...");
        goto error_out;
    }

    /* Intializes random number generator */
    srand(time(NULL));

    // load game main menu
    GameContextLoadMenu(&ctx);

    // all resources loaded normally -> start drawing the window and the game
    while (!glfwWindowShouldClose(window))
    {
        ctx.currentMode->processInput(ctx.currentMode);
        glfwGetWindowSize(window, &wWidth, &wHeight);

        if (ctx.state == GameStateMenu)
        {
            GameMenuSetup(&ctx.menu, &ctx.state, wWidth, wHeight, ctx.modes, ctx.modes_count, &ctx.currentMode);
        }

        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (ctx.state == GameStateMenu)
        {
            GameMenuRender(&ctx.menu);
        }
        else if (ctx.state == GameStatePlaying)
        {
            ctx.currentMode->draw(ctx.currentMode, &ctx.state, wWidth, wHeight);
        }
        else if (ctx.state == GameStateOver)
        {
            GameContextGameOver(&ctx);
            ctx.state = GameStateMenu;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    goto out;

error_out:
    LogError("errors occured... stopping the game");
out:
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
