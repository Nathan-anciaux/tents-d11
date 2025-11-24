#include "game_ext.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

game game_new_ext(uint, uint, square*, uint*, uint*, bool) {
  game g = NULL;
  return g;
}

game game_new_empty_ext(uint, uint, bool) {
  game g = NULL;
  return g;
}

uint game_nb_rows(cgame) {
  uint u = 0;
  return u;
}

uint game_nb_cols(cgame) {
  uint u = 0;
  return u;
}

bool game_is_wrapping(cgame) { return true; }

void game_undo(game) { return; }

void game_redo(game) { return; }
