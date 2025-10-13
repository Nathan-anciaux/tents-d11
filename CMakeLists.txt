cmake_minimum_required(VERSION 3.5)
link_directories(${CMAKE_SOURCE_DIR})
project(game_text C)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_executable(game_text game_text.c)
target_link_libraries(game_text libgame.a)
add_executable(game_test_maxjulienne game_test_maxjulienne.c)
add_test(test_maxjulienne_dummy ./game_test_maxjulienne dummy)

add_executable(game_test_edejardin game_test_edejardin.c)
add_test(test_edejardin_dummy ./game_test_edejardin dummy)
