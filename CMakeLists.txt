cmake_minimum_required(VERSION 3.13)
set(CMAKE_C_STANDARD 99)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(sdl-ping-pong C)
add_executable(sdl-ping-pong src/main.c src/logic.c src/logic.h src/render.c src/render.h)

target_link_libraries(sdl-ping-pong SDL2 SDL2_ttf dl pthread)

# TODO: sudo apt-get install libsdl2-ttf-dev libsdl2-dev
