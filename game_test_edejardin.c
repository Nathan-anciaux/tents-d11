#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "game_tools.h"

bool test_game_new(void);
bool test_game_new_empty(void);
bool test_game_copy(void);
bool test_game_equal(void);
bool test_game_delete(void);
bool test_game_play_move(void);
bool test_game_check_move(void);
bool test_game_save(void);
bool test_game_load(void);
int test_dummy(void);

int test_dummy() { return 1; }

// Test game_new, if dummy made up board is alike to a game_new created board
bool test_game_new(void)
{
  // initializing a default board
  uint tents_row[] = {1, 2, 1, 2, 2, 0, 1, 3};
  uint tents_col[] = {3, 0, 2, 1, 2, 1, 2, 1};
  square squares[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY,
                      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};

  game g_new = game_new(squares, tents_row, tents_col);
  game g_ref = game_default();
  if (g_new == NULL) {
    game_delete(g_ref);
    return 0;
  }
  if (g_new == NULL || g_ref == NULL) {
    game_delete(g_new);
    return 0;
  }
  if (!game_equal(g_new, g_ref)) {
    game_delete(g_new);
    game_delete(g_ref);
    return 0;
  }
  game_delete(g_ref);

  // verify if we have a link between board and locals variables
  squares[0] = EMPTY;
  tents_row[0] = 99;
  tents_col[0] = 99;
  if (game_get_square(g_new, 0, 0) != TREE) {
    game_delete(g_new);
    return 0;
  }
  if (game_get_expected_nb_tents_row(g_new, 0) != 1) {
    game_delete(g_new);
    return 0;
  }
  if (game_get_expected_nb_tents_col(g_new, 0) != 3) {
    game_delete(g_new);
    return 0;
  }
  tents_row[0] = 1;
  tents_col[0] = 3;

  // check the expected number of tents in a row
  for (int i = 0; i < game_nb_rows(g_new); i++) {
    if (tents_row[i] != game_get_expected_nb_tents_row(g_new, i)) {
      game_delete(g_new);
      return 0;
    }
  }

  // same for column
  for (int i = 0; i < game_nb_cols(g_new); i++) {
    if (tents_col[i] != game_get_expected_nb_tents_col(g_new, i)) {
      game_delete(g_new);
      return 0;
    }
  }

  game_delete(g_new);

  // initialising illegal board
  squares[0] = TREE;
  tents_row[0] = 7;
  tents_col[0] = 7;
  g_new = game_new(squares, tents_row, tents_col);
  g_ref = game_default();
  if (g_new == NULL) {
    game_delete(g_ref);
    return 0;
  }
  if (g_new == NULL || g_ref == NULL) {
    game_delete(g_new);
    return 0;
  }

  // check if both board are different
  if (game_equal(g_new, g_ref)) {
    game_delete(g_new);
    game_delete(g_ref);
    return 0;
  }

  // Purpose: forbid illegal number (like: >game_nb_row / col)
  for (int i = 0; i < game_nb_rows(g_new); i++) {
    if (tents_row[i] != game_get_expected_nb_tents_row(g_new, i)) {
      game_delete(g_new);
      game_delete(g_ref);
      return 0;
    }
  }

  for (int i = 0; i < game_nb_cols(g_new); i++) {
    if (tents_col[i] != game_get_expected_nb_tents_col(g_new, i)) {
      game_delete(g_new);
      game_delete(g_ref);
      return 0;
    }
  }

  game_delete(g_new);
  game_delete(g_ref);
  return 1;
}

bool test_game_new_empty(void)
{
  game g = game_new_empty();
  if (g == NULL) return 0;

  // check if every squares are empty
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) != EMPTY) return 0;
    }
  }
  // check expected tents in every row / coll (we expect 0 for an empty board)
  for (int i = 0; i < game_nb_rows(g); i++) {
    if (game_get_expected_nb_tents_row(g, i) != 0) return 0;
  }

  for (int i = 0; i < game_nb_cols(g); i++) {
    if (game_get_expected_nb_tents_col(g, i) != 0) return 0;
  }

  if (game_get_current_nb_tents_all(g) != 0) return 0;

  game_delete(g);
  return 1;
}

// auxiliary function to free game_copy board test
static void free_copy(game g1, game g2, game g3, game g1_copy, game g2_copy, game g3_copy)
{
  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g1_copy);
  game_delete(g2_copy);
  game_delete(g3_copy);
  return;
}

// Purpose: test every possibility and see if game_copy works (both board are equal)
bool test_game_copy()
{
  game g1 = game_default();
  game g1_copy = game_copy(g1);
  game g2 = game_new_empty();
  game g2_copy = game_copy(g2);
  game g3 = game_default_solution();
  game g3_copy = game_copy(g3);

  for (int i = 0; i < game_nb_cols(g1); i++) {
    for (int j = 0; j < game_nb_rows(g1); j++) {
      if (game_get_square(g1, i, j) != game_get_square(g1_copy, i, j) ||
          game_get_square(g2, i, j) != game_get_square(g2_copy, i, j) ||
          game_get_square(g3, i, j) != game_get_square(g3_copy, i, j)) {
        free_copy(g1, g2, g3, g1_copy, g2_copy, g3_copy);
        return 0;
      }
    }
  }

  if (g1 == g1_copy || g3 == g3_copy || g2 == g2_copy) {
    free_copy(g1, g2, g3, g1_copy, g2_copy, g3_copy);
    return 0;
  }

  free_copy(g1, g2, g3, g1_copy, g2_copy, g3_copy);
  return 1;
}

// auxiliary function to free game_equal board test
static void free_equal(game g1, game g2)
{
  game_delete(g1);
  game_delete(g2);
  return;
}

bool test_game_equal(void)
{
  game g1 = game_default();
  game g2 = game_default();
  if (g1 == NULL) {
    free_equal(g1, g2);
    return 0;
  }
  if (g2 == NULL) {
    game_delete(g1);
    game_delete(g2);
    return 0;
  }
  if (!game_equal(g1, g2)) {
    free_equal(g1, g2);
    return 0;
  }
  // check equality of current nb tents
  if (game_get_current_nb_tents_all(g1) != game_get_current_nb_tents_all(g2)) {
    free_equal(g1, g2);
    return 0;
  }
  if (game_get_expected_nb_tents_all(g1) != game_get_expected_nb_tents_all(g2)) {
    free_equal(g1, g2);
    return 0;
  }
  game g_copy = game_copy(g1);
  if (g_copy == NULL) {
    free_equal(g1, g2);
    return 0;
  }
  if (!game_equal(g1, g_copy)) {
    game_delete(g_copy);
    free_equal(g1, g2);
    return 0;
  }
  game_delete(g_copy);

  // test fill and what happen if we change a special square in one board (no more equal)
  game g_fill = game_copy(g1);

  if (g_fill == NULL) {
    free_equal(g1, g2);
    return 0;
  }
  game_fill_grass(g_fill);
  game gf1 = game_copy(g_fill);

  if (gf1 == NULL) {
    free_equal(g1, g2);
    game_delete(gf1);
    game_delete(g_fill);
    return 0;
  }

  if (!game_equal(gf1, g_fill)) {
    free_equal(g1, g2);
    game_delete(g_fill);
    game_delete(gf1);
    return 0;
  }
  uint test[3] = {TENT, EMPTY};
  for (uint i = 0; i < 2; i++) {
    game_play_move(g_fill, 0, 4, test[i]);
    if (game_equal(gf1, g_fill)) {
      free_equal(g1, g2);
      game_delete(g_fill);
      game_delete(gf1);
      return 0;
    }
  }

  game_delete(g_fill);
  game_delete(gf1);

  // test change of expected nb_row / tents
  for (uint i = 0; i < game_nb_rows(g2); i++) {
    game_set_expected_nb_tents_row(g2, i, 99);
    if (game_equal(g2, g1)) {
      free_equal(g1, g2);
      return 0;
    }
  }

  for (uint i = 0; i < game_nb_cols(g2); i++) {
    game_set_expected_nb_tents_col(g2, i, 99);
    if (game_equal(g2, g1)) {
      free_equal(g1, g2);
      return 0;
    }
  }

  free_equal(g1, g2);
  return 1;
}

bool test_game_delete(void)
{
  game g1 = game_default();
  game g2 = game_new_empty();
  game g3 = game_default();
  game_play_move(g3, 0, 1, TENT);
  game g4 = game_copy(g3);
  if (g1 == NULL || g2 == NULL || g3 == NULL || g4 == NULL) return 0;

  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g4);
  return 1;
}

bool test_game_play_move(void)
{
  game g = game_default();
  if (g == NULL) return 0;

  // bunch of test for every possibility
  int test[9] = {EMPTY, TENT, TENT, EMPTY, GRASS, GRASS, TENT, GRASS, EMPTY};
  int row0, col1, all;

  // check if a move is forbidden --> board has change ?
  bool interdit;
  for (int i = 0; i < 9; i++) {
    square old = game_get_square(g, 0, 1);
    square new = test[i];

    interdit = game_check_move(g, 0, 1, new);

    row0 = game_get_current_nb_tents_row(g, 0);
    col1 = game_get_current_nb_tents_col(g, 1);
    all = game_get_current_nb_tents_all(g);

    game_play_move(g, 0, 1, new);
    if (interdit) {
      if (game_get_square(g, 0, 1) != new) return 0;
      int diff = (new == TENT) - (old == TENT);
      if (game_get_current_nb_tents_row(g, 0) != row0 + diff) return 0;
      if (game_get_current_nb_tents_col(g, 1) != col1 + diff) return 0;
      if (game_get_current_nb_tents_all(g) != all + diff) return 0;
    }
  }

  game_delete(g);
  return 1;
}

bool test_game_check_move(void)
{
  game g = game_default();
  if (g == NULL) return 0;
  game g1 = game_default();
  game_play_move(g1, 0, 1, GRASS);
  // bunch of test coord and square
  int test_tents[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1};
  int test[15] = {TENT, GRASS, EMPTY, TREE, TREE, TREE, EMPTY, TENT, TENT, EMPTY, GRASS, GRASS, TENT, GRASS, EMPTY};
  // test something on a tree or tree on something --> illegal
  for (int i = 0, j = 0; j < 6 && i < 6; i++, j = j + 2) {
    if (game_check_move(g1, test_tents[j], test_tents[j + 1], test[i])) {
      game_delete(g);
      game_delete(g1);
      return 0;
    }
  }
  // test something on something
  for (int i = 6; i < 15; i++) {
    if (!game_check_move(g, 0, 4, test[i])) {
      game_delete(g);
      game_delete(g1);
      return 0;
    }
  }
  game_delete(g);
  game_delete(g1);
  return 1;
}

bool test_game_save(void)
{
  char* filename = "../saves/test_game_save1.txt";
  // test basic default game
  game g = game_default();
  game_save(g, filename);
  game gl = game_load(filename);
  if (gl == NULL) {
    return false;
  }
  if (!game_equal(g, gl)) {
    game_delete(g);
    game_delete(gl);
    remove(filename);
    return false;
  }

  // what if i play a move?
  game_play_move(g, 1, 1, TENT);
  game_save(g, filename);
  // test if differences
  if (game_equal(g, gl)) {
    game_delete(g);
    game_delete(gl);
    remove(filename);
    return false;
  }
  game_delete(gl);
  gl = game_load(filename);
  // test if no diffferences
  if (gl == NULL || !game_equal(g, gl)) {
    game_delete(g);
    game_delete(gl);
    remove(filename);
    return false;
  }

  game_delete(g);
  game_delete(gl);
  remove(filename);
  // what if i play on a rectangle game?
  {
    filename = "../saves/test_game_save2.txt";
    uint tents_row[] = {3, 1, 2, 1, 1, 2, 1, 2};
    uint tents_col[] = {3, 1, 2, 1, 1, 2, 1, 0, 2};
    square squares[] = {GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, TREE,
                        GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
                        GRASS, GRASS, TREE,  TREE,  GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS,
                        GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, TREE,  GRASS,
                        GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS,
                        GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS};
    game g2 = game_new_ext(8, 9, squares, tents_row, tents_col, false);
    if (g2 == NULL) {
      return false;
    }
    game_save(g2, filename);
    game gl2 = game_load(filename);
    if (gl2 == NULL || !game_equal(g2, gl2)) {
      game_delete(g2);
      game_delete(gl2);
      remove(filename);
      return false;
    }
    game_delete(g2);
    game_delete(gl2);
    remove(filename);
  }
  // what if i play on a rectangle game?
  {
    filename = "../saves/test_game_save3.txt";
    uint tents_row[] = {3, 1, 2, 1, 1, 2, 1, 2, 1};
    uint tents_col[] = {3, 1, 2, 1, 1, 2, 1, 0};
    square squares[] = {GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, TREE,  GRASS,
                        GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS,
                        TREE,  GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS,
                        GRASS, GRASS, GRASS, TREE,  GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, TREE,
                        GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS,
                        GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS, GRASS};
    game g3 = game_new_ext(9, 8, squares, tents_row, tents_col, false);
    if (g3 == NULL) {
      return false;
    }
    game_save(g3, filename);
    game gl3 = game_load(filename);
    if (gl3 == NULL || !game_equal(g3, gl3)) {
      game_delete(g3);
      game_delete(gl3);
      remove(filename);
      return false;
    }
    game_delete(g3);
    game_delete(gl3);
    remove(filename);
  }

  return true;
}

bool test_game_load(void)
{
  char* filename = "../saves/test_load1.txt";
  FILE* f = fopen(filename, "w");
  if (f == NULL) {
    return false;
  }

  fprintf(f, "2 2 0\n");
  fprintf(f, "0 1\n");
  fprintf(f, "1 0\n");
  fprintf(f, "G G\n");
  fprintf(f, "T X\n");
  fclose(f);

  game g = game_load(filename);
  if (g == NULL) {
    remove(filename);
    return false;
  }

  if (game_nb_rows(g) != 2 || game_nb_cols(g) != 2) {
    game_delete(g);
    remove(filename);
    return false;
  }

  if (game_get_square(g, 0, 0) != GRASS || game_get_square(g, 0, 1) != GRASS || game_get_square(g, 1, 0) != TENT ||
      game_get_square(g, 1, 1) != TREE) {
    printf("probleme de carré");
    return false;
  }
  if (game_get_expected_nb_tents_row(g, 1) != 1 || game_get_expected_nb_tents_col(g, 0) != 1) {
    printf("expected");
    return false;
  }

  game_delete(g);
  remove(filename);

  game g1 = game_load("../saves/existepas.txt");
  if (g1 != NULL) {
    return false;
  }
  return true;
}

void usage(int argc, char* argv[])
{
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

int main(int argc, char* argv[])
{
  if (argc == 1) usage(argc, argv);
  bool ok = 0;
  if (strcmp("dummy", argv[1]) == 0) {
    ok = test_dummy();
  }
  if (strcmp("game_new", argv[1]) == 0) {
    ok = test_game_new();
  }
  if (strcmp("game_new_empty", argv[1]) == 0) {
    ok = test_game_new_empty();
  }
  if (strcmp("game_copy", argv[1]) == 0) {
    ok = test_game_copy();
  }
  if (strcmp("game_equal", argv[1]) == 0) {
    ok = test_game_equal();
  }
  if (strcmp("game_delete", argv[1]) == 0) {
    ok = test_game_delete();
  }
  if (strcmp("game_play_move", argv[1]) == 0) {
    ok = test_game_play_move();
  }
  if (strcmp("game_check_move", argv[1]) == 0) {
    ok = test_game_check_move();
  }
  if (strcmp("game_save", argv[1]) == 0) {
    ok = test_game_save();
  }
  if (strcmp("game_load", argv[1]) == 0) {
    ok = test_game_load();
  }
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS \n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE \n", argv[1]);
    return EXIT_FAILURE;
  }
}
