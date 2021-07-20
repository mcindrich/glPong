cmake_minimum_required(VERSION 3.0.0)
project(glPong C)

include_directories(
    "lib"
    "deps/libs/cglm/include/"
    "deps/libs/Nuklear/"
    "deps/libs/cgl/lib/"
    "deps/libs/stb/"
)

link_directories(
    "deps/libs/cglm/build/"
    "deps/libs/cgl/build/"
)

set(
    LIB_SOURCES
    "lib/glPong/ball.c"
    "lib/glPong/paddle.c"
    "lib/glPong/drawable.c"
) 

set(
    SOURCES
    "app/main.c"
)

find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(
    ${CMAKE_PROJECT_NAME}
    ${SOURCES}
    ${LIB_SOURCES}
)

target_link_libraries(
    ${CMAKE_PROJECT_NAME} 
    PRIVATE GLEW::GLEW glfw cglm cgl
)