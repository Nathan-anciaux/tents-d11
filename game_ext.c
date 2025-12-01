#include "game_ext.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_struct.h"

static setsquare recup_val(stack *p) {
  setsquare val = pop(p);
  push(val, p);
  return val;
}

game game_new_ext(uint nb_rows, uint nb_cols, square *squares,
                  uint *nb_tents_row, uint *nb_tents_col, bool wrapped) {
  // memory allocation for all g attribut's
  game g = malloc(sizeof(struct game_s));
  if (g == NULL) {
    free(g);
    exit(EXIT_FAILURE);
  }
  g->wrapped = wrapped;
  g->width = nb_cols;
  g->height = nb_rows;
  g->elem = malloc(g->width * g->height * sizeof(square));
  g->tab = malloc(g->height * sizeof(square *));
  g->nb_tent_col_expected = malloc(g->width * sizeof(uint));
  g->nb_tent_row_expected = malloc(g->height * sizeof(uint));
  g->undo = create_stack();
  g->redo = create_stack();

  if (g->tab == NULL || g->nb_tent_col_expected == NULL ||
      g->nb_tent_row_expected == NULL || g->elem == NULL) {
    game_delete(g);
    exit(EXIT_FAILURE);
  }

  // Filling the table with all expected values (squares)
  for (uint i = 0; i < g->width * g->height; i++) {
    g->elem[i] = squares[i];
  }

  for (uint i = 0; i < g->width; i++) {
    g->nb_tent_col_expected[i] = nb_tents_col[i];
  }
  for (uint i = 0; i < g->height; i++) {
    g->nb_tent_row_expected[i] = nb_tents_row[i];
  }
  // Filling
  for (int i = 0; i < g->height; i++) {
    g->tab[i] = &g->elem[i * g->width];
  }

  return g;
}

game game_new_empty_ext(uint height, uint width, bool wrapping) {
  // Allocation of the element to be completed and to use with game_new
  square *tab = calloc(height * width, sizeof(square));
  uint *expected_tent_row = calloc(height, sizeof(uint));
  uint *expected_tent_col = calloc(width, sizeof(uint));

  // Create the game with all parameters empty (thanks to calloc)
  game g = game_new_ext(height, width, tab, expected_tent_row,
                        expected_tent_col, wrapping);
  free(tab);
  free(expected_tent_row);
  free(expected_tent_col);
  return g;
}

uint game_nb_rows(cgame g) { return g->height; }

uint game_nb_cols(cgame g) { return g->width; }

bool game_is_wrapping(cgame g) { return g->wrapped; }

void game_undo(game g) {
  if (!isempty_stack(g->undo)) {
    if ((int)recup_val(g->undo).col == -20 &&
        (int)recup_val(g->undo).square == GRASS) {
      int nb_square = (int)recup_val(g->undo).row;
      pop(g->undo);
      setsquare square;
      for (int k = 0; k < nb_square; k++) {
        square = pop(g->undo);
        // j'undo en mettant l'ancien etat dans redo
        setsquare squareplayed = {square.row, square.col,
                                  game_get_square(g, square.row, square.col)};
        game_set_square(g, square.row, square.col, square.square);
        push(squareplayed, g->redo);
      }
      setsquare square_idx = {nb_square, -20, GRASS};
      push(square_idx, g->redo);
    } else {
      setsquare square;
      square = pop(g->undo);
      // j'undo en mettant l'ancien etat dans redo
      setsquare squareplayed = {square.row, square.col,
                                game_get_square(g, square.row, square.col)};
      game_set_square(g, square.row, square.col, square.square);
      push(squareplayed, g->redo);
    }
  }
  return;
}

void game_redo(game g) {
  if (!isempty_stack(g->redo)) {
    if ((int)recup_val(g->redo).col == -20 &&
        (int)recup_val(g->redo).square == GRASS) {
      int nb_square = (int)recup_val(g->redo).row;
      pop(g->redo);
      for (int k = 0; k < nb_square; k++) {
        setsquare square;
        square = pop(g->redo);
        // j'edo en mettant l'ancien etat dans undo
        setsquare squareplayed = {square.row, square.col,
                                  game_get_square(g, square.row, square.col)};
        game_set_square(g, square.row, square.col, square.square);
        push(squareplayed, g->undo);
      }
      setsquare square_idx = {nb_square, -20, GRASS};
      push(square_idx, g->undo);
    } else {
      setsquare square;
      square = pop(g->redo);
      // j'undo en mettant l'ancien etat dans redo
      setsquare squareplayed = {square.row, square.col,
                                game_get_square(g, square.row, square.col)};
      game_set_square(g, square.row, square.col, square.square);
      push(squareplayed, g->undo);
    }
  }
  return;
}
