// game_struct.h
#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__
#include "game.h"
#include "game_aux.h"

struct game_s {
  // pour les nombres de tentes attendues colonne et  ligne : 2 tableaux
  square **tab;  // tableau simple pour la grille
  square *elem;
  uint *nb_tent_row_expected;
  uint *nb_tent_col_expected;
};
typedef struct game_s *game;

#endif
