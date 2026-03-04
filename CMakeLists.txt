cmake_minimum_required(VERSION 3.5)
link_directories(${CMAKE_SOURCE_DIR})
project(game_text C)
enable_testing()

set(CMAKE_C_FLAGS "-Wall -std=c99")            # options de compilation génériques
set(CMAKE_C_FLAGS_DEBUG "-g -O0")              # options de compilation en mode Debug
set(CMAKE_C_FLAGS_RELEASE "-O3")               # options de compilation en mode Release

add_library(game STATIC         
        game.c
        game_aux.c
        game_ext.c
        queue.c
        game_private.c
        game_tools.c
)
include(CTest)
target_include_directories(game PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# folder copy
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/saves/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/saves/)

add_executable(game_text game_text.c)
target_link_libraries(game_text game)
add_executable(game_random game_text.c)
target_link_libraries(game_random game)
add_executable(game_solve game_text.c)
target_link_libraries(game_solve game)

add_executable(game_test_maxjulienne game_test_maxjulienne.c)
target_link_libraries(game_test_maxjulienne game)
add_test(test_maxjulienne_dummy ./game_test_maxjulienne dummy)
add_test(test_game_won ./game_test_maxjulienne game_won)
add_test(test_game_fill_grass ./game_test_maxjulienne game_fill_grass)
add_test(test_game_restart ./game_test_maxjulienne game_restart)
add_test(test_game_is_neigh ./game_test_maxjulienne game_is_neigh)
add_test(test_game_default ./game_test_maxjulienne game_default)
add_test(test_game_default_solution ./game_test_maxjulienne game_default_solution)
add_test(test_game_print ./game_test_maxjulienne game_print)
add_test(test_game_get_adjacent_square ./game_test_maxjulienne game_get_adjacent_square)
add_test(test_game_nb_neigh ./game_test_maxjulienne game_nb_neigh)
add_test(test_game_new_empty_ext ./game_test_maxjulienne game_new_empty_ext)
add_test(test_game_get_expected_nb_tents_all ./game_test_maxjulienne game_get_expected_nb_tents_all)

add_executable(game_test_nanciaux game_test_nanciaux.c)
target_link_libraries(game_test_nanciaux game)
add_test(test_nanciaux_dummy ./game_test_nanciaux dummy)
add_test(test_game_set_square ./game_test_nanciaux game_set_square)
add_test(test_game_set_expected_nb_tents_row ./game_test_nanciaux game_set_expected_nb_tents_row)
add_test(test_game_set_expected_nb_tents_col ./game_test_nanciaux game_set_expected_nb_tents_col)
add_test(test_game_get_square ./game_test_nanciaux game_get_square)
add_test(test_game_get_expected_nb_tents_row ./game_test_nanciaux game_get_expected_nb_tents_row)
add_test(test_game_get_expected_nb_tents_col ./game_test_nanciaux game_get_expected_nb_tents_col)
add_test(test_game_get_expected_nb_tents_all ./game_test_nanciaux game_get_expected_nb_tents_all)
add_test(test_game_get_current_nb_tents_row ./game_test_nanciaux  game_get_current_nb_tents_row)
add_test(test_game_get_current_nb_tents_col ./game_test_nanciaux  game_get_current_nb_tents_col)
add_test(test_game_get_current_nb_tents_all ./game_test_nanciaux  game_get_current_nb_tents_all)
add_test(test_game_redo ./game_test_nanciaux game_redo)
add_test(test_game_undo ./game_test_nanciaux game_undo)
add_test(test_game_is_wrapping ./game_test_nanciaux game_is_wrapping)
add_test(test_game_nb_cols ./game_test_nanciaux game_nb_cols)
add_test(test_game_nb_rows ./game_test_nanciaux game_nb_rows)
add_test(test_game_new_empty_ext ./game_test_nanciaux game_new_empty_ext)
add_test(test_game_new_ext ./game_test_nanciaux game_new_ext)
add_test(test_game_solve ./game_test_nanciaux game_solve)

add_executable(game_test_edejardin game_test_edejardin.c)
target_link_libraries(game_test_edejardin game)
add_test(test_game_new ./game_test_edejardin game_new)
add_test(test_game_new_empty ./game_test_edejardin game_new_empty)
add_test(test_game_copy ./game_test_edejardin game_copy)
add_test(test_game_equal ./game_test_edejardin game_equal)
add_test(test_game_delete ./game_test_edejardin game_delete)
add_test(test_game_play_move ./game_test_edejardin game_play_move)
add_test(test_game_check_move ./game_test_edejardin game_check_move)
add_test(test_game_save ./game_test_edejardin game_save)
add_test(test_game_load ./game_test_edejardin game_load)
add_test(test_game_nbsol ./game_test_edejardin game_nbsol)
add_test(test_edejardin_dummy ./game_test_edejardin dummy)