#include <glPong/game/mode.h>
#include <GLFW/glfw3.h>

int SpeedPlusPlusModeInit(GameMode *mode, GLFWwindow *win);
int SpeedPlusPlusModeProcessInput(GameMode *mode);
int SpeedPlusPlusModeDraw(GameMode *mode, GameState *state, int wWidth, int wHeight);
int SpeedPlusPlusModeDelete(GameMode *mode);

GameMode speedPlusPlusGameMode = {
    .name = "SpeedPlusPlus",
    .desc = "The speed of the ball increases as the paddles touch it - last one standing wins",
    .init = SpeedPlusPlusModeInit,
    .processInput = SpeedPlusPlusModeProcessInput,
    .draw = SpeedPlusPlusModeDraw,
    .delete = SpeedPlusPlusModeDelete,
};

int SpeedPlusPlusModeInit(GameMode *mode, GLFWwindow *win)
{
    int err = 0;
    // init the paddle, ball etc.
    return err;
}

int SpeedPlusPlusModeProcessInput(GameMode *mode)
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

int SpeedPlusPlusModeDraw(GameMode *mode, GameState *state, int wWidth, int wHeight)
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

    int collidesWithPaddle = 0;
    if (mode->ball.draw.speed < 0)
    {
        collidesWithPaddle = BallCheckPaddleCollision(&mode->ball, &mode->lPaddle);
        if (collidesWithPaddle)
        {
            mode->ball.draw.speed += 0.001f;
        }
    }
    else
    {
        collidesWithPaddle = BallCheckPaddleCollision(&mode->ball, &mode->rPaddle);
        if (collidesWithPaddle)
        {
            mode->ball.draw.speed -= 0.001f;
        }
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

int SpeedPlusPlusModeDelete(GameMode *mode)
{
    int err = 0;
    return err;
}
