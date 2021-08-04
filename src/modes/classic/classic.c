#include <glPong/game/mode.h>
#include <GLFW/glfw3.h>

int ClassicModeInit(GameMode *mode, GLFWwindow *win);
int ClassicModeProcessInput(GameMode *mode);
int ClassicModeDraw(GameMode *mode, GameState *state, int wWidth, int wHeight);
int ClassicModeDelete(GameMode *mode);

GameMode classicGameMode = {
    .name = "Classic",
    .desc = "A classic implementation of pong - 2 players playing until one misses the ball",
    .init = ClassicModeInit,
    .processInput = ClassicModeProcessInput,
    .draw = ClassicModeDraw,
    .delete = ClassicModeDelete,
};

int ClassicModeInit(GameMode *mode, GLFWwindow *win)
{
    int err = 0;
    // init the paddle, ball etc.
    return err;
}

int ClassicModeProcessInput(GameMode *mode)
{
    int err = 0;
    GLFWwindow *window = mode->win;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        PaddleMove(&mode->rPaddle, DirectionUp);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        PaddleMove(&mode->rPaddle, DirectionDown);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        PaddleMove(&mode->rPaddle, DirectionLeft);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        PaddleMove(&mode->rPaddle, DirectionRight);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        PaddleMove(&mode->lPaddle, DirectionUp);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        PaddleMove(&mode->lPaddle, DirectionDown);
    }
    return err;
}

int ClassicModeDraw(GameMode *mode, GameState *state, int wWidth, int wHeight)
{
    int err = 0;
    int over = 0;
    if (mode->ball.direction == DirectionNone)
    {
        mode->ball.direction = (rand() % 2 == 0) ? DirectionLeft : DirectionRight;
        const Direction dir = mode->ball.direction;
        glm_vec2_copy((vec2){1, 0}, mode->ball.dirUnitVec);
    }
    // set resolution for all drawables
    glm_vec2((vec2){wWidth, wHeight}, mode->lPaddle.draw.uResolution);
    glm_vec2((vec2){wWidth, wHeight}, mode->rPaddle.draw.uResolution);
    glm_vec2((vec2){wWidth, wHeight}, mode->ball.draw.uResolution);

    if (mode->ball.draw.speed < 0)
    {
        BallCheckPaddleCollision(&mode->ball, &mode->lPaddle);
    }
    else
    {
        BallCheckPaddleCollision(&mode->ball, &mode->rPaddle);
    }
    over = BallCheckWallCollision(&mode->ball);
    if (over)
    {
        *state = GameStateOver;
    }

    // draw objects
    BallDraw(&mode->ball);
    PaddleDraw(&mode->lPaddle, DirectionLeft);
    PaddleDraw(&mode->rPaddle, DirectionRight);
    return err;
}

int ClassicModeDelete(GameMode *mode)
{
    int err = 0;
    return err;
}
