cmake_minimum_required(VERSION 3.0.0)
project(glPong C)

include_directories(
    "src"
    "deps/libs/cglm/include/"
    "deps/libs/Nuklear/"
    "deps/libs/cgl/lib/"
    "deps/libs/stb/"
    "deps/libs/cJSON/"
)

link_directories(
    "deps/libs/cglm/build/"
    "deps/libs/cgl/build/"
)

set(
    SOURCES
    "src/main.c"
    "src/glPong/ball.c"
    "src/glPong/paddle.c"
    "src/glPong/drawable.c"
    "src/glPong/game/menu.c"
    "src/glPong/game/context.c"
    "src/glPong/game/mode.c"
    "src/glPong/game/config.c"
    "deps/libs/cJSON/cJSON.c"
)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(
    ${CMAKE_PROJECT_NAME}
    ${SOURCES}
)

add_subdirectory(
    "src/modes/classic"
)

add_subdirectory(
    "src/modes/speedplusplus"
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME} 
    PRIVATE GLEW::GLEW glfw cglm cgl game-mode-classic game-mode-speedplusplus
)
