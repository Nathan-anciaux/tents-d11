// game_struct.h
#ifndef __GAMESTRUCT_H__
#define __GAMESTRUCT_H__
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

struct game_s {
  // pour les nombres de tentes attendues colonne et  ligne : 2 tableaux
  square **tab; // Tableau qui indique le début de chaque ligne
  square *elem; // Tableau de tous les 
  uint *nb_tent_row_expected;
  uint *nb_tent_col_expected;
  int width;
  int height;
};
typedef struct game_s *game;

#endif
