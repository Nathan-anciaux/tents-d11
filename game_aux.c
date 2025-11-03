// game_aux.c
#include "game_struct.h"

void game_print(cgame g)
// Prints a game as text on the standard output stream.
{
    return;
}

game game_default(void)
// Creates the default game
{
    return;
}

game game_default_solution(void)
// Creates the default game solution.
{
    return;
}

bool game_get_ajacent_square(cgame g, uint i, uint j, direction d, uint *pi_next, uint *pj_next)
// Gets the coordinate of the adjacent square in a given direction.
{
    return;
}

bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag)
// Determines if a neighboring square has a given value
{
    return;
}