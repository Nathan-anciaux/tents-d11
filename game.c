// game.c
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_struct.h"

static bool on_board(cgame g, int i, int j) { return i >= 0 && i < g->height && j >= 0 && j < g->width; }

void game_delete(game g)
// Deletes the game and frees the allocated memory
{
  free(g->tab);
  free(g->elem);
  free(g->nb_tent_col_expected);
  free(g->nb_tent_row_expected);
  free(g->redo);
  free(g->undo);
  free(g);
  return;
}

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col)
// Creates a new game with default size and initializes it
{
  // memory allocation for all g attribut's
  game g = malloc(sizeof(struct game_s));
  if (g == NULL) {
    free(g);
    exit(EXIT_FAILURE);
  }

  g->elem = malloc(DEFAULT_SIZE * DEFAULT_SIZE * sizeof(square));
  g->tab = malloc(DEFAULT_SIZE * sizeof(square*));
  g->nb_tent_col_expected = malloc(DEFAULT_SIZE * sizeof(uint));
  g->nb_tent_row_expected = malloc(DEFAULT_SIZE * sizeof(uint));
  g->width = DEFAULT_SIZE;
  g->height = DEFAULT_SIZE;
  g->redo = create_stack();
  g->undo = create_stack();

  if (g->tab == NULL || g->nb_tent_col_expected == NULL || g->nb_tent_row_expected == NULL || g->elem == NULL) {
    game_delete(g);
    exit(EXIT_FAILURE);
  }

  // Filling the table with all expected values (squares)
  for (uint i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    g->elem[i] = squares[i];
  }

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    g->nb_tent_col_expected[i] = nb_tents_col[i];
    g->nb_tent_row_expected[i] = nb_tents_row[i];
  }
  // Filling
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    g->tab[i] = &g->elem[i * DEFAULT_SIZE];
  }

  return g;
}

game game_new_empty(void)  // Attention ici on utilise l'implémentaion 1d du tableau
// Creates a new empty game with defaut size
{
  // Allocation of the element to be completed and to use with game_new
  square* tab = calloc(DEFAULT_SIZE * DEFAULT_SIZE, sizeof(square));
  uint* expected_tent_row = calloc(DEFAULT_SIZE, sizeof(uint));
  uint* expected_tent_col = calloc(DEFAULT_SIZE, sizeof(uint));

  game g = game_new(tab, expected_tent_row, expected_tent_col);
  free(tab);
  free(expected_tent_row);
  free(expected_tent_col);
  return g;
}

game game_copy(cgame g)
// 	Duplicates a game
{
  game g2 = game_new(g->elem, g->nb_tent_row_expected, g->nb_tent_col_expected);

  return g2;
}

bool game_equal(cgame g1, cgame g2)
// Tests if two games are equal
{
  /*
  int j = 0;
  for (int i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) return 0;
    if (j % (DEFAULT_SIZE - 1) == 0) i++;
  }*/

  if (g1 == NULL || g2 == NULL) return g1 == g2;
  if (g1->height != g2->height || g1->width != g2->width) {
    return false;
  }

  for (int i = 0; i < g1->height; i++) {
    for (int j = 0; j < g1->width; j++) {
      if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) return 0;
    }
  }

  if (game_get_current_nb_tents_all(g1) != game_get_current_nb_tents_all(g2)) return 0;
  if (game_get_expected_nb_tents_all(g1) != game_get_expected_nb_tents_all(g2)) return 0;

  for (int i = 0; i < g1->height; i++) {
    if (game_get_expected_nb_tents_row(g1, i) != game_get_expected_nb_tents_row(g2, i)) return 0;
    if (game_get_current_nb_tents_row(g1, i) != game_get_current_nb_tents_row(g2, i)) return 0;
  }

  for (int i = 0; i < g1->width; i++) {
    if (game_get_current_nb_tents_col(g1, i) != game_get_current_nb_tents_col(g2, i)) return 0;
    if (game_get_expected_nb_tents_col(g1, i) != game_get_expected_nb_tents_col(g2, i)) return 0;
  }
  return 1;
}

void game_set_square(game g, uint i, uint j, square s)
// Sets the value of a given square
{
  if (on_board(g, i, j)) g->tab[i][j] = s;
  return;
}

square game_get_square(cgame g, uint i, uint j)
// Gets the value of a given square
{
  if (g == NULL) return 0;
  if (j < 0 || i < 0 || i >= g->height || j >= g->width) exit(EXIT_FAILURE);
  return g->tab[i][j];
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents)
// Sets the expected number of tents in a given row.
{
  g->nb_tent_row_expected[i] = nb_tents;
  return;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents)
// Sets the expected number of tents in a given column.
{
  g->nb_tent_col_expected[j] = nb_tents;
  return;
}

uint game_get_expected_nb_tents_row(cgame g, uint i)
// Gets the expected number of tents in a given row
{
  if (g == NULL) return 0;
  return g->nb_tent_row_expected[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j)
// Gets the expected number of tents in a given column
{
  if (g == NULL) return 0;
  return g->nb_tent_col_expected[j];
}

uint game_get_expected_nb_tents_all(cgame g)
// Gets the expected number of tents on the whole grid.
{
  uint u = 0;
  for (int i = 0; i < g->width; i++) {
    u = u + g->nb_tent_row_expected[i];
  }
  return u;
}

uint game_get_current_nb_tents_row(cgame g, uint i)
// Gets the current number of tents in a given row.
{
  if (g == NULL) return 0;
  if (i >= g->width) {
    return 0;
  }

  uint u = 0;
  for (int j = 0; j < g->height; j++) {
    if (game_get_square(g, i, j) == TENT) {
      u++;
    }
  }
  return u;
}

uint game_get_current_nb_tents_col(cgame g, uint j)
// Gets the current number of tents in a given column.
{
  if (g == NULL) return 0;
  if (j >= g->height) {
    return 0;
  }
  uint u = 0;
  for (int i = 0; i < g->width; i++) {
    if (game_get_square(g, i, j) == TENT) {
      u++;
    }
  }
  return u;
}

uint game_get_current_nb_tents_all(cgame g)
// Gets the current number of tents on the whole grid.
{
  if (g == NULL) return 0;
  uint u = 0;
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_get_square(g, i, j) == TENT) {
        u++;
      }
    }
  }
  return u;
}

void game_play_move(game g, uint i, uint j, square s)
// Plays a move in a given square
{
  if (game_check_move(g, i, j, s)) {
    // on reinitialise le redo
    delete_stack(g->redo);
    g->redo = create_stack();

    // on met letat de l'ancien carré dans le undo
    setsquare squareplayed = {i, j, game_get_square(g, i, j)};
    push(squareplayed, g->undo);
    game_set_square(g, i, j, s);
  }
  return;
}

bool game_check_move(cgame g, uint i, uint j, square s)
// Checks if a given move in a square is legal
{
  if (g == NULL) return 0;
  if (i >= g->height || j >= g->width) return false;
  if (s == TREE) return false;
  if (s == BIGTREE) return false;
  if (game_get_square(g, i, j) == TREE) return false;
  if (game_get_square(g, i, j) == BIGTREE) return false;

  return 1;
}

bool game_won(cgame g)
// Checks if the game is won
{
  /*
  // game_default_solution test
  game gds = game_default_solution();
  if (game_equal(g, gds)) {
    game_delete(gds);
    return true;
  }
  game_delete(gds);
  // game_new_empty test
  game gne = game_new_empty();
  if (game_equal(g, gne)) {
    game_delete(gne);
    return true;
  }
  game_delete(gne);
  // game_default test
  game gd = game_default();
  if (game_equal(g, gd)) {
    game_delete(gd);
    return false;
  }
  game_delete(gd);
  */

  // Rule 1
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_get_square(g, i, j) == TENT && game_is_neigh(g, i, j, TENT, true)) {
        return false;
      }
    }
  }

  // Rule 2
  for (int i = 0; i < g->width; i++) {
    if (!(game_get_current_nb_tents_col(g, i) == game_get_expected_nb_tents_col(g, i))) {
      return false;
    }
  }
  for (int i = 0; i < g->height; i++) {
    if (!(game_get_current_nb_tents_row(g, i) == game_get_expected_nb_tents_row(g, i))) {
      return false;
    }
  }

  /*
  // Rule 3
  int sTENT = 0;
  int sTREE = 0;
  int sBIGTREE = 0;
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_get_square(g, i, j) == TENT) {
        sTENT++;
      }
      if (game_get_square(g, i, j) == TREE) {
        sTREE++;
      }
      if (game_get_square(g, i, j) == BIGTREE) {
        sBIGTREE++;
      }
    }
  }
  if (sTENT != sTREE + 2 * sBIGTREE) {
    return false;
  }*/

  // Rule 4
  // on check si chaque bigtree est collé a exactement 2 tents
  int compteur = 0;
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_get_square(g, i, j) == TENT) {
        if (!game_is_neigh(g, i, j, TREE, false) && !game_is_neigh(g, i, j, BIGTREE, false)) return false;
      }
      if (game_get_square(g, i, j) == BIGTREE) {
        compteur = game_nb_neigh(g, i, j, TENT, false);
        if (compteur != 2) {
          return false;
        }
      }
      compteur = 0;
      square s = game_get_square(g, i, j);
      if (s == TREE) {
        if (!game_is_neigh(g, i, j, TENT, false)) return false;
      }
    }
  }

  return true;
}

void game_fill_grass(game g)
// Fills the board with grass
{
  // Replace all squares that are EMPTY with GRASS. Here we do 2 loop to be more
  // understandable and to use already done functions
  int size = 0;
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        game_play_move(g, i, j, GRASS);
        size++;
      }
    }
  }
  setsquare square_idx = {size, -20, GRASS};
  push(square_idx, g->undo);
  return;
}

void game_restart(game g)
// Restarts a game.
{
  // Replace all squares with EMPTY if there are not TREE
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (!(game_get_square(g, i, j) == TREE)) {
        game_set_square(g, i, j, EMPTY);
      }
    }
  }
  return;
}