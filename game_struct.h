// game_struct.h
#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "stack.h"

struct game_s {
  // pour les nombres de tentes attendues colonne et  ligne : 2 tableaux
  square** tab;  // Tableau qui indique le début de chaque ligne
  square* elem;  // Tableau de tous les
  uint* nb_tent_row_expected;
  uint* nb_tent_col_expected;
  uint width;
  uint height;
  stack* undo;
  stack* redo;
  bool wrapped;
};
// typedef struct game_s *game;

void game_delete_ext(game g);

#endif
