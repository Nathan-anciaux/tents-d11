// game.c
#include "game_struct.h"

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col)
// Creates a new game with default size and initializes it
{
  game g;
  return g;
}

game game_new_empty(void)
// Creates a new empty game with defaut size
{
  return;
}

game game_copy(cgame g)
// 	Duplicates a game
{
  return;
}

bool game_equal(cgame g1, cgame g2)
// Tests if two games are equal
{
  return 1;
}

void game_delete(game g)
// Deletes the game and frees the allocated memory
{
  return;
}

void game_set_square(game g, uint i, uint j, square s)
// Sets the value of a given square
{
  return;
}

square game_get_square(cgame g, uint i, uint j)
// Gets the value of a given square
{
  return;
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents)
// Sets the expected number of tents in a given row.
{
  return;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents)
// Sets the expected number of tents in a given column.
{
  return;
}

uint game_get_expected_nb_tents_row(cgame g, uint i)
// Gets the expected number of tents in a given row
{
  return;
}

uint game_get_expected_nb_tents_col(cgame g, uint j)
// Gets the expected number of tents in a given column
{
  return;
}

uint game_get_expected_nb_tents_all(cgame g)
// Gets the expected number of tents on the whole grid.
{
  return;
}

uint game_get_current_nb_tents_row(cgame g, uint i)
// Gets the current number of tents in a given row.
{
  return;
}

uint game_get_current_nb_tents_col(cgame g, uint j)
// Gets the current number of tents in a given column.
{
  return;
}

uint game_get_current_nb_tents_all(cgame g)
// Gets the current number of tents on the whole grid.
{
  return;
}

void game_play_move(game g, uint i, uint j, square s)
// Plays a move in a given square
{
  return;
}

bool game_check_move(cgame g, uint i, uint j, square s)
// Checks if a given move in a square is legal
{
  return;
}

bool game_won(cgame g)
// Checks if the game is won
{
  return;
}

void game_fill_grass(game g)
// Fills the board with grass
{
  return;
}

void game_restart(game g)
// Restarts a game.
{
  return;
}