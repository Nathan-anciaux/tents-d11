// game_aux.c
#include <stdio.h>

#include "game_struct.h"

void game_print(cgame g)
// Prints a game as text on the standard output stream.
{
  return;
}

game game_default(void)
// Creates the default game
{
  game g = NULL;
  return g;
}

game game_default_solution(void)
// Creates the default game solution.
{
  game g = NULL;
  return g;
}

bool game_get_ajacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next)
// Gets the coordinate of the adjacent square in a given direction.
{
  return 1;
}

bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag)
// Determines if a neighboring square has a given value
{
  return 1;
}