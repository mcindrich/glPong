cmake_minimum_required(VERSION 3.0.0)
project(glPong C)

include_directories(
    "lib"
    "deps/libs"
)

set(
    LIB_SOURCES
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
    PRIVATE GLEW::GLEW glfw
)