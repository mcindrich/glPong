#include <stdio.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glPong.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

static void framebufferSizeCB(GLFWwindow *window, int w, int h);
static void processInput(GLFWwindow *window);

int main()
{
    GLFWwindow *window = NULL;
    GLint glewStatus = 0;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "glPong", NULL, NULL);
    if (!window)
    {
        log_err("Unable to create a GLFW window... exiting");
        glfwTerminate();
        return -1;
    }

    // window created -> make context current and initialize GLEW
    glfwMakeContextCurrent(window);

    glewStatus = glewInit();

    if (glewStatus != GLEW_OK)
    {
        log_err("Unable to init GLEW... exiting");
        glfwTerminate();
        return -1;
    }

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCB);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.3f, 0.2f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw after drawing the background color

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

static void framebufferSizeCB(GLFWwindow *window, int w, int h)
{
    // change viewport to match new window size
    glViewport(0, 0, w, h);
}

static void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, 1);
    }
}