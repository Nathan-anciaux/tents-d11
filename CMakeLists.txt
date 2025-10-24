cmake_minimum_required(VERSION 3.5)
link_directories(${CMAKE_SOURCE_DIR})
project(game_text C)
enable_testing()

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_executable(game_text game_text.c)
target_link_libraries(game_text libgame.a )

add_executable(game_test_maxjulienne game_test_maxjulienne.c)
target_link_libraries(game_test_maxjulienne libgame.a)
add_test(test_maxjulienne_dummy ./game_test_maxjulienne dummy)
add_test(test_game_won ./game_test_maxjulienne game_won)
add_test(test_game_fill_grass ./game_test_maxjulienne game_fill_grass)
add_test(test_game_restart ./game_test_maxjulienne game_restart)
add_test(test_game_is_neigh ./game_test_maxjulienne game_is_neigh)
add_test(test_game_default ./game_test_maxjulienne game_default)
add_test(test_game_default_solution ./game_test_maxjulienne game_default_solution)
add_test(test_game_print ./game_test_maxjulienne game_print)
add_test(test_game_get_ajacent_square ./game_test_maxjulienne game_get_ajacent_square)
add_test(test_game_get_expected_nb_tents_all ./game_test_maxjulienne game_get_expected_nb_tents_all)




add_executable(game_test_nanciaux game_test_nanciaux.c)
add_test(test_nanciaux_dummy ./game_test_nanciaux dummy)

add_executable(game_test_edejardin game_test_edejardin.c)
target_link_libraries(game_test_edejardin libgame.a)
add_test(test_game_new ./game_test_edejardin game_new)
add_test(test_game_new_empty ./game_test_edejardin game_new_empty)
add_test(test_game_copy ./game_test_edejardin game_copy)
add_test(test_game_equal ./game_test_edejardin game_equal)
add_test(test_game_delete ./game_test_edejardin game_delete)
add_test(test_game_play_move ./game_test_edejardin game_play_move)
add_test(test_game_check_move ./game_test_edejardin game_check_move)
add_test(test_game_get_current_nb_tents_all ./game_test_edejardin game_get_current_nb_tents_all)
add_test(test_edejardin_dummy ./game_test_edejardin dummy)