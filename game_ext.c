#include "game_ext.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_struct.h"

game game_new_ext(uint width, uint height, square *squares, uint *nb_tents_row,
                  uint *nb_tents_col, bool wrapped) {
  // memory allocation for all g attribut's
  game g = malloc(sizeof(struct game_s));
  if (g == NULL) {
    free(g);
    exit(EXIT_FAILURE);
  }
  g->wrapped = wrapped;
  g->elem = malloc(width * height * sizeof(square));
  g->tab = malloc(height * sizeof(square *));
  g->nb_tent_col_expected = malloc(width * sizeof(uint));
  g->nb_tent_row_expected = malloc(height * sizeof(uint));
  g->width = width;
  g->height = height;

  if (g->tab == NULL || g->nb_tent_col_expected == NULL ||
      g->nb_tent_row_expected == NULL || g->elem == NULL) {
    game_delete(g);
    exit(EXIT_FAILURE);
  }

  // Filling the table with all expected values (squares)
  for (uint i = 0; i < width * height; i++) {
    g->elem[i] = squares[i];
  }

  for (uint i = 0; i < width; i++) {
    g->nb_tent_col_expected[i] = nb_tents_col[i];
  }
  for (uint i = 0; i < height; i++) {
    g->nb_tent_row_expected[i] = nb_tents_col[i];
  }
  // Filling
  for (int i = 0; i < height; i++) {
    g->tab[i] = &g->elem[i * DEFAULT_SIZE];
  }

  return g;
}

game game_new_empty_ext(uint height, uint width, bool wrapping) {
  // Allocation of the element to be completed and to use with game_new
  square *tab = calloc(height * width, sizeof(square));
  uint *expected_tent_row = calloc(height, sizeof(uint));
  uint *expected_tent_col = calloc(width, sizeof(uint));

  // Create the game with all parameters empty (thanks to calloc)
  game g = game_new_ext(width, height, tab, expected_tent_row,
                        expected_tent_col, wrapping);
  free(tab);
  free(expected_tent_row);
  free(expected_tent_col);
  return g;
}

uint game_nb_rows(cgame g) { return g->height; }

uint game_nb_cols(cgame g) { return g->width; }

bool game_is_wrapping(cgame g) { return g->wrapped; }

void game_undo(game) { return; }

void game_redo(game) { return; }
