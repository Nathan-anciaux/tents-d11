// game.c
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_struct.h"

static void game_pointer_validity_verification(game g)
{
  if (g == NULL) {
    free(g);
    fprintf(stderr, "ERROR, game pointer is NULL");
    exit(EXIT_FAILURE);
  }
}

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col)
// Creates a new game with default size and initializes it
{
  // memory allocation for all g attribut's
  game g = malloc(sizeof(struct game_s));

  game_pointer_validity_verification(
      g);  // We test it only here beacause each time we want to create a game we use this function

  g->tab = malloc(DEFAULT_SIZE * DEFAULT_SIZE * sizeof(square));
  g->nb_tent_col_expected = malloc(DEFAULT_SIZE * sizeof(uint));
  g->nb_tent_row_expected = malloc(DEFAULT_SIZE * sizeof(uint));

  if (g->tab == NULL || g->nb_tent_col_expected == NULL || g->nb_tent_row_expected == NULL) {
    free(g->tab);
    free(g->nb_tent_col_expected);
    free(g->nb_tent_row_expected);

    fprintf(stderr, "ERROR");
    exit(EXIT_FAILURE);
  }

  // Filling the table with all expected values (squares)
  for (uint i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    g->tab[i] = squares[i];
  }

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    g->nb_tent_col_expected[i] = nb_tents_col[i];
    g->nb_tent_row_expected[i] = nb_tents_row[i];
  }

  return g;
}

game game_new_empty(void)
// Creates a new empty game with defaut size
{
  // Allocation of the element to be completed and to use with game_new
  square* tab = malloc(sizeof(square) * DEFAULT_SIZE * DEFAULT_SIZE);
  uint* expected_tent_row = malloc(sizeof(uint) * DEFAULT_SIZE);
  uint* expected_tent_col = malloc(sizeof(uint) * DEFAULT_SIZE);

  // Filling the tab with EMPTY values
  for (int i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    tab[i] = EMPTY;
  }
  // Set all the expected tents to 0
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    expected_tent_col[i] = 0;
    expected_tent_row[i] = 0;
  }
  // Create the game with all parameters empty
  game g = game_new(tab, expected_tent_row, expected_tent_col);
  return g;
}

game game_copy(cgame g)
// 	Duplicates a game
{
  // Get all the g attributes and create a new game g2 with those
  /*game g2 = game_new_empty();

  for (int k = 0; k < DEFAULT_SIZE; k++) {
    game_set_expected_nb_tents_row(g2, k, g->nb_tent_row_expected[k]);
    game_set_expected_nb_tents_col(g2, k, g->nb_tent_col_expected[k]);
  }

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_square(g2, i, j, g->tab[i * DEFAULT_SIZE + j]);
    }
  }*/
  game g2 = game_new(g->tab, g->nb_tent_row_expected, g->nb_tent_col_expected);

  return g2;
}

/*EVAANNNN*/
bool game_equal(cgame g1, cgame g2)
// Tests if two games are equal
{
  int j = 0;
  for (int i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    if (game_get_square(g1, i, j) != game_get_square(g2, i, j)) return 0;
    if (i % (DEFAULT_SIZE - 1) == 0) j++;
  }

  if (game_get_current_nb_tents_all(g1) == game_get_current_nb_tents_all(g2)) return 0;
  if (game_get_expected_nb_tents_all(g1) == game_get_expected_nb_tents_all(g2)) return 0;
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_current_nb_tents_row(g1, i) == game_get_current_nb_tents_row(g2, i)) return 0;
    if (game_get_current_nb_tents_col(g1, i) == game_get_current_nb_tents_col(g2, i)) return 0;
    if (game_get_expected_nb_tents_col(g1, i) == game_get_expected_nb_tents_col(g2, i)) return 0;
    if (game_get_expected_nb_tents_col(g1, i) == game_get_expected_nb_tents_col(g2, i)) return 0;
  }
  return 1;
}

void game_delete(game g)
// Deletes the game and frees the allocated memory
{
  free(g->tab);
  free(g->nb_tent_col_expected);
  free(g->nb_tent_row_expected);
  free(g);
  return;
}

void game_set_square(game g, uint i, uint j, square s)
// Sets the value of a given square
{
  g->tab[i * DEFAULT_SIZE + j] = s;
  return;
}

square game_get_square(cgame g, uint i, uint j)
// Gets the value of a given square
{
  return g->tab[i * DEFAULT_SIZE + j];
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
  return g->nb_tent_row_expected[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j)
// Gets the expected number of tents in a given column
{
  return g->nb_tent_col_expected[j];
}

uint game_get_expected_nb_tents_all(cgame g)
// Gets the expected number of tents on the whole grid.
{
  uint u = 0;
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    u = u + g->nb_tent_row_expected[i];
  }
  return u;
}

uint game_get_current_nb_tents_row(cgame g, uint i)
// Gets the current number of tents in a given row.
{
  uint u = 0;
  for (int j = 0; j < DEFAULT_SIZE; j++) {
    if (game_get_square(g, i, j) == TENT) {
      u++;
    }
  }
  return u;
}

uint game_get_current_nb_tents_col(cgame g, uint j)
// Gets the current number of tents in a given column.
{
  uint u = 0;
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_square(g, i, j) == TENT) {
      u++;
    }
  }
  return u;
}

uint game_get_current_nb_tents_all(cgame g)
// Gets the current number of tents on the whole grid.
{
  uint u = 0;
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
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
  if (game_check_move(g, i, j, s)) g->tab[i * DEFAULT_SIZE + j] = s;
  return;
}

bool game_check_move(cgame g, uint i, uint j, square s)
// Checks if a given move in a square is legal
{
  if (game_is_neigh(g, i, j, TENT, true)) {
    return false;
  }
  if (game_get_current_nb_tents_row(g, i) == game_get_expected_nb_tents_row(g, i)) {
    return false;
  }
  if (game_get_current_nb_tents_col(g, i) == game_get_expected_nb_tents_col(g, i)) {
    return false;
  }
  if (!game_is_neigh(g, i, j, TREE, true)) {
    return false;
  }
  return true;
}

bool game_won(cgame g)
// Checks if the game is won
{
  // game_default_solution test
  if (game_equal(g, game_default_solution())) {
    return true;
  }
  // game_new_empty test
  if (game_equal(g, game_new_empty())) {
    return true;
  }
  // game_default test
  if (game_equal(g, game_default())) {
    return false;
  }
  // Rule 1
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) == TENT && game_is_neigh(g, i, j, TENT, true)) {
        return false;
      }
    }
  }
  // Rule 2
  int s = 0;
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_current_nb_tents_row(g, i) == game_get_expected_nb_tents_row(g, i) &&
        game_get_current_nb_tents_col(g, i) == game_get_expected_nb_tents_col(g, i)) {
      s++;
    }
  }
  if (s != DEFAULT_SIZE) {
    return false;
  }
  // Rule 3
  int sTENT = 0;
  int sTREE = 0;
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) == TENT) {
        sTENT++;
      }
      if (game_get_square(g, i, j) == TREE) {
        sTREE++;
      }
    }
  }
  if (sTENT != sTREE) {
    return false;
  }
  // Rule 4
  int nb_adj_TENT_TREE = 0;
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) == TENT && game_is_neigh(g, i, j, TREE, false)) {
        nb_adj_TENT_TREE++;
      }
    }
  }
  if (nb_adj_TENT_TREE != sTENT) {
    return false;
  }
  return true;
}

void game_fill_grass(game g)
// Fills the board with grass
{
  // Replace all squares that are EMPTY with GRASS. Here we do 2 loop to be more understandable and to use already done
  // functions
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        game_set_square(g, i, j, GRASS);
      }
    }
  }
  return;
}

void game_restart(game g)
// Restarts a game.
{
  // Replace all squares with EMPTY if there are not TREE
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (!(game_get_square(g, i, j) == TREE)) {
        game_set_square(g, i, j, EMPTY);
      }
    }
  }
  return;
}