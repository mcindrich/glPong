cmake_minimum_required(VERSION 3.0.0)
project(game-mode-classic C)

add_library(
    ${PROJECT_NAME}
    "classic.c"
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE GLEW::GLEW glfw cglm cgl
)