#ifndef __STRUCT_S__
#define __STRUCT_S__

#include <stdbool.h>

#include "game.h"
#include "queue.h"

struct game_s {
  // uint width_height;  // Dimensions de la grille
  uint width;
  uint height;
  square **grille;     // Taille width * height
  uint *expected_row;  // Nombre de tents par row (de taille height)
  uint *expected_col;  // Nombre de tents par col (de taille witdh)
  queue *undo;
  queue *redo;
  bool is_wrap;
};

#endif /* __STRUCT_S__  */