cmake_minimum_required(VERSION 3.5)
link_directories(${CMAKE_SOURCE_DIR})
project(game_text C)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_executable(game_text game_text.c)
target_link_libraries(game_text libgame.a)
