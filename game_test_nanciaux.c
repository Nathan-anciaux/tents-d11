#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

bool test_game_redo();

bool test_game_undo();

bool test_game_is_wrapping();

bool test_game_nb_cols();

bool test_game_nb_rows();

bool test_game_new_empty_ext();

bool test_game_new_ext();

bool test_game_set_square();

bool test_game_set_expected_nb_tents_row();

bool test_game_set_expected_nb_tents_col();

bool test_game_get_square();

bool test_game_get_expected_nb_tents_row();

bool test_game_get_expected_nb_tents_col();

bool test_game_get_expected_nb_tents_all();

bool test_game_get_current_nb_tents_row();

bool test_game_get_current_nb_tents_col();

bool test_game_get_current_nb_tents_all();

bool test_game_get_ajacent_square();

bool test_game_random();

bool test_game_solve();

int test_dummy() { return 1; }

bool test_game_set_square()
{
  // test sur game empty
  game g = game_new_empty();
  if (g == NULL) {
    return false;
  }
  uint L[] = {TENT, GRASS, EMPTY, TREE};
  for (uint i = 0; i < game_nb_rows(g); i++) {
    for (uint j = 0; j < game_nb_cols(g); j++) {
      for (uint k = 0; k < 4; k++) {
        game_set_square(g, i, j, L[k]);
        if (game_get_square(g, i, j) != L[k]) {
          game_delete(g);
          return false;
        }
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_set_expected_nb_tents_row()
{
  // test on game empty
  game g = game_new_empty();
  if (g == NULL) {
    return false;
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      game_set_expected_nb_tents_row(g, i, j);
      if (game_get_expected_nb_tents_row(g, i) != j) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_set_expected_nb_tents_col()
{
  // test on game empty
  game g = game_new_empty();
  if (g == NULL) {
    return false;
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      game_set_expected_nb_tents_col(g, i, j);
      if (game_get_expected_nb_tents_col(g, i) != j) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_get_square()
{
  // test on game empty and on new game
  game g_empty = game_new_empty();
  int L[] = {TENT, GRASS, EMPTY, TREE};
  if (g_empty == NULL) {
    return false;
  }
  for (int i = 0; i < game_nb_rows(g_empty); i++) {
    for (int j = 0; j < game_nb_cols(g_empty); j++) {
      for (int k = 0; k < 4; k++) {
        game_set_square(g_empty, i, j, L[k]);
        if (game_get_square(g_empty, i, j) != L[k]) {
          game_delete(g_empty);
          return false;
        }
      }
    }
  }
  uint nb_tents_row[] = {1, 2, 1, 2, 2, 0, 1, 3};
  uint nb_tents_col[] = {3, 0, 2, 1, 2, 1, 2, 1};
  square squares[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY,
                      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};
  game g = game_new(squares, nb_tents_row, nb_tents_col);
  if (g == NULL) {
    game_delete(g_empty);
    return false;
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      int k = i * DEFAULT_SIZE + j;
      if (game_get_square(g, i, j) != squares[k]) {
        game_delete(g);
        game_delete(g_empty);
        return false;
      }
    }
  }
  game_delete(g);
  game_delete(g_empty);
  return true;
}

bool test_game_get_expected_nb_tents_row()
{
  // test on game empty and on new game
  game g_empty = game_new_empty();
  if (g_empty == NULL) {
    return false;
  }
  for (int i = 0; i < game_nb_rows(g_empty); i++) {
    for (int j = 0; j < game_nb_cols(g_empty); j++) {
      game_set_expected_nb_tents_row(g_empty, i, j);
      if (game_get_expected_nb_tents_row(g_empty, i) != j) {
        game_delete(g_empty);
        return false;
      }
    }
  }
  uint nb_tents_row[] = {1, 2, 1, 2, 2, 0, 1, 3};
  uint nb_tents_col[] = {3, 0, 2, 1, 2, 1, 2, 1};
  square squares[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY,
                      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};
  game g = game_new(squares, nb_tents_row, nb_tents_col);
  if (g == NULL) {
    game_delete(g_empty);
    return false;
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row[i]) {
      game_delete(g);
      game_delete(g_empty);
      return false;
    }
  }
  game_delete(g);
  game_delete(g_empty);
  return true;
}

bool test_game_get_expected_nb_tents_col()
{
  // test on game empty and on new game
  game g_empty = game_new_empty();
  if (g_empty == NULL) {
    return false;
  }
  for (int i = 0; i < game_nb_rows(g_empty); i++) {
    for (int j = 0; j < game_nb_cols(g_empty); j++) {
      game_set_expected_nb_tents_col(g_empty, i, j);
      if (game_get_expected_nb_tents_col(g_empty, i) != j) {
        game_delete(g_empty);
        return false;
      }
    }
  }
  uint nb_tents_row[] = {1, 2, 1, 2, 2, 0, 1, 3};
  uint nb_tents_col[] = {3, 0, 2, 1, 2, 1, 2, 1};
  square squares[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY,
                      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};
  game g = game_new(squares, nb_tents_row, nb_tents_col);
  if (g == NULL) {
    game_delete(g_empty);
    return false;
  }
  for (int i = 0; i < game_nb_cols(g); i++) {
    if (game_get_expected_nb_tents_col(g, i) != nb_tents_col[i]) {
      game_delete(g);
      game_delete(g_empty);
      return false;
    }
  }
  game_delete(g);
  game_delete(g_empty);
  return true;
}

bool test_game_get_expected_nb_tents_all()
{
  // test on game empty ,on new game and on game default
  game g_empty = game_new_empty();
  game g = game_default();
  if (g_empty == NULL) {
    game_delete(g);
    return false;
  }
  if (g == NULL) {
    game_delete(g_empty);
    return false;
  }
  int s = 0;
  if (game_get_expected_nb_tents_all(g_empty) != 0) {
    game_delete(g_empty);
    game_delete(g);
    return false;
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    s = s + game_get_expected_nb_tents_row(g, i);
  }
  if (game_get_expected_nb_tents_all(g) != s) {
    game_delete(g_empty);
    game_delete(g);
    return false;
  }
  uint nb_tents_row[] = {1, 2, 1, 2, 2, 0, 1, 3};
  uint nb_tents_col[] = {3, 0, 2, 1, 2, 1, 2, 1};
  square squares[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY,
                      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};
  game g_new = game_new(squares, nb_tents_row, nb_tents_col);
  if (g_new == NULL) {
    game_delete(g_empty);
    game_delete(g);
    return false;
  }
  s = 0;
  for (int i = 0; i < game_nb_rows(g); i++) {
    s = s + game_get_expected_nb_tents_row(g_new, i);
  }
  if (game_get_expected_nb_tents_all(g_new) != s) {
    game_delete(g_empty);
    game_delete(g_new);
    game_delete(g);
    return false;
  }
  game_delete(g_empty);
  game_delete(g_new);
  game_delete(g);
  return true;
}

bool test_game_get_current_nb_tents_row()
{
  // test on game default
  game g = game_default();
  int s = 0;
  if (g == NULL) {
    return false;
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    s = 0;
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == TENT) {
        s++;
      }
      if (game_get_current_nb_tents_row(g, i) != s) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}

bool test_game_get_current_nb_tents_col()
{
  // test on game default
  game g = game_default();
  int s = 0;
  if (g == NULL) {
    return false;
  }
  for (int j = 0; j < game_nb_cols(g); j++) {
    s = 0;
    for (int i = 0; i < game_nb_rows(g); i++) {
      if (game_get_square(g, i, j) == TENT) {
        s++;
      }
      if (game_get_current_nb_tents_col(g, j) != s) {
        game_delete(g);
        return false;
      }
    }
  }
  game_delete(g);
  return true;
}
bool test_game_get_current_nb_tents_all()
{
  // test on game default and game empty
  game g = game_default();
  game g_empty = game_new_empty();
  int s = 0;
  if (g == NULL) {
    game_delete(g_empty);
    return false;
  }
  if (g_empty == NULL) {
    game_delete(g);
    return false;
  }
  if (game_get_current_nb_tents_all(g_empty) != 0) {
    game_delete(g_empty);
    game_delete(g);
    return false;
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) == TENT) {
        s++;
      }
    }
  }
  if (game_get_current_nb_tents_all(g) != s) {
    game_delete(g_empty);
    game_delete(g);
    return false;
  }
  game_delete(g_empty);
  game_delete(g);
  return true;
}

bool test_game_new_ext()
{
  // initializing a default game
  uint tents_row[] = {1, 3, 1, 2, 2, 0, 1, 3};
  uint nb_rows = 8;
  uint tents_col[] = {4, 0, 2, 1, 2, 1, 2, 1, 0};
  uint nb_cols = 9;
  bool wrap = true;
  square squares[] = {BIGTREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      TREE,    EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
                      EMPTY,   EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY};

  game g_new_ext = game_new_ext(nb_rows, nb_cols, squares, tents_row, tents_col, wrap);
  if (g_new_ext == NULL) {
    return false;
  }

  // dimension test
  if (nb_rows != game_nb_rows(g_new_ext)) {
    game_delete(g_new_ext);
    return false;
  }
  if (nb_cols != game_nb_cols(g_new_ext)) {
    game_delete(g_new_ext);
    return false;
  }

  // wrapping test
  if (wrap != game_is_wrapping(g_new_ext)) {
    game_delete(g_new_ext);
    return false;
  }

  // square test
  for (int i = 0; i < game_nb_rows(g_new_ext); i++) {
    for (int j = 0; j < game_nb_cols(g_new_ext); j++) {
      if (game_get_square(g_new_ext, i, j) != squares[i * game_nb_cols(g_new_ext) + j]) {
        game_delete(g_new_ext);
        return false;
      }
    }
  }

  // test data independence
  squares[0] = EMPTY;
  tents_row[0] = 99;
  tents_col[0] = 99;
  if (game_get_square(g_new_ext, 0, 0) != BIGTREE) {
    game_delete(g_new_ext);
    return false;
  }
  if (game_get_expected_nb_tents_row(g_new_ext, 0) != 1) {
    game_delete(g_new_ext);
    return false;
  }
  if (game_get_expected_nb_tents_col(g_new_ext, 0) != 4) {
    game_delete(g_new_ext);
    return false;
  }
  tents_row[0] = 1;
  tents_col[0] = 4;
  squares[0] = BIGTREE;

  // checks if each row have correct nb of tents
  for (int i = 0; i < game_nb_rows(g_new_ext); i++) {
    if (tents_row[i] != game_get_expected_nb_tents_row(g_new_ext, i)) {
      game_delete(g_new_ext);
      return false;
    }
  }

  // checks if each col have correct nb of tents
  for (int i = 0; i < game_nb_cols(g_new_ext); i++) {
    if (tents_col[i] != game_get_expected_nb_tents_col(g_new_ext, i)) {
      game_delete(g_new_ext);
      return false;
    }
  }

  // test equal with a another game with different parameters (expected false)
  tents_row[0] = 7;
  tents_col[0] = 7;
  game g_new_ext_false = game_new_ext(nb_rows, nb_cols, squares, tents_row, tents_col, wrap);
  if (g_new_ext_false == NULL) {
    game_delete(g_new_ext);
    return false;
  }
  if (game_equal(g_new_ext, g_new_ext_false)) {
    game_delete(g_new_ext);
    game_delete(g_new_ext_false);
    return false;
  }
  game_delete(g_new_ext);
  game_delete(g_new_ext_false);
  return true;
}

bool test_game_redo()
{
  // initializing an empty game
  uint nb_rows = 8;
  uint nb_cols = 9;
  bool wrap = true;
  game g_new_empty_ext = game_new_empty_ext(nb_rows, nb_cols, wrap);

  if (g_new_empty_ext == NULL) {
    return false;
  }

  // Game redo test
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      game_play_move(g_new_empty_ext, i, j, TENT);
      game_undo(g_new_empty_ext);
      game_redo(g_new_empty_ext);
      if (game_get_square(g_new_empty_ext, i, j) != TENT) {
        game_delete(g_new_empty_ext);
        return false;
      }
      game_set_square(g_new_empty_ext, i, j, EMPTY);
    }
  }
  game_delete(g_new_empty_ext);
  return true;
}

bool test_game_undo()
{
  // initializing an empty game
  uint nb_rows = 8;
  uint nb_cols = 9;
  bool wrap = true;
  game g_new_empty_ext = game_new_empty_ext(nb_rows, nb_cols, wrap);

  if (g_new_empty_ext == NULL) {
    return false;
  }

  // Game undo test
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      game_play_move(g_new_empty_ext, i, j, TENT);
      game_undo(g_new_empty_ext);
      if (game_get_square(g_new_empty_ext, i, j) != EMPTY) {
        game_delete(g_new_empty_ext);
        return false;
      }
    }
  }
  game_delete(g_new_empty_ext);
  return true;
}

bool test_game_is_wrapping()
{
  // initializing a default game
  uint tents_row[] = {1, 3, 1, 2, 2, 0, 1, 3};
  uint nb_rows = 8;
  uint tents_col[] = {4, 0, 2, 1, 2, 1, 2, 1, 0};
  uint nb_cols = 9;
  bool wrap = true;
  square squares[] = {BIGTREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      TREE,    EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
                      EMPTY,   EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY};

  game g_new_ext = game_new_ext(nb_rows, nb_cols, squares, tents_row, tents_col, wrap);

  if (g_new_ext == NULL) {
    return false;
  }

  if (!game_is_wrapping(g_new_ext)) {
    game_delete(g_new_ext);
    return false;
  }

  // Change le bool wrap
  wrap = false;
  game g_new_ext_2 = game_new_ext(nb_rows, nb_cols, squares, tents_row, tents_col, wrap);

  if (g_new_ext_2 == NULL) {
    game_delete(g_new_ext);
    return false;
  }

  if (game_is_wrapping(g_new_ext_2)) {
    game_delete(g_new_ext);
    game_delete(g_new_ext_2);
    return false;
  }
  game_delete(g_new_ext);
  game_delete(g_new_ext_2);
  return true;
}

bool test_game_nb_cols()
{
  // initializing a default game
  uint tents_row[] = {1, 3, 1, 2, 2, 0, 1, 3};
  uint nb_rows = 8;
  uint tents_col[] = {4, 0, 2, 1, 2, 1, 2, 1, 0};
  uint nb_cols = 9;
  bool wrap = true;
  square squares[] = {BIGTREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      TREE,    EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
                      EMPTY,   EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY};

  game g_new_ext = game_new_ext(nb_rows, nb_cols, squares, tents_row, tents_col, wrap);

  if (g_new_ext == NULL) {
    return false;
  }

  if (game_nb_cols(g_new_ext) != nb_cols) {
    game_delete(g_new_ext);
    return false;
  }
  game_delete(g_new_ext);
  return true;
}

bool test_game_nb_rows()
{
  // initializing a default game
  uint tents_row[] = {1, 3, 1, 2, 2, 0, 1, 3};
  uint nb_rows = 8;
  uint tents_col[] = {4, 0, 2, 1, 2, 1, 2, 1, 0};
  uint nb_cols = 9;
  bool wrap = true;
  square squares[] = {BIGTREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      TREE,    EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,
                      EMPTY,   EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY,   EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY};

  game g_new_ext = game_new_ext(nb_rows, nb_cols, squares, tents_row, tents_col, wrap);
  if (game_nb_rows(g_new_ext) != nb_rows) {
    game_delete(g_new_ext);
    return false;
  }
  game_delete(g_new_ext);
  return true;
}

bool test_game_new_empty_ext()
{
  // initializing an empty game
  uint nb_rows = 8;
  uint nb_cols = 9;
  bool wrap = true;
  game g_new_empty_ext = game_new_empty_ext(nb_rows, nb_cols, wrap);

  if (g_new_empty_ext == NULL) {
    return false;
  }

  // check if the tab tents row is empty
  for (int i = 0; i < nb_rows; i++) {
    if (game_get_expected_nb_tents_row(g_new_empty_ext, i) != 0) {
      game_delete(g_new_empty_ext);
      return false;
    }
  }

  // check if the tab tents col is empty
  for (int j = 0; j < nb_cols; j++) {
    if (game_get_expected_nb_tents_col(g_new_empty_ext, j) != 0) {
      game_delete(g_new_empty_ext);
      return false;
    }
  }

  // check if the game is empty
  for (int i = 0; i < nb_rows; i++) {
    for (int j = 0; j < nb_cols; j++) {
      if (game_get_square(g_new_empty_ext, i, j) != 0) {
        game_delete(g_new_empty_ext);
        return false;
      }
    }
  }
  game_delete(g_new_empty_ext);
  return true;
}

bool test_game_random()
{
  // test won on solved random game (wrapping off/on)
  for (int i = 0; i < 2; i++) {
    game gr = game_random(10, 10, i, 6, 4, true);
    if (!game_won(gr)) {
      game_delete(gr);
      return 0;
    }
    game_delete(gr);
  }

  // test won on not solved random game (wrapping off/on)
  for (int i = 0; i < 2; i++) {
    game gr = game_random(10, 10, i, 6, 4, false);
    if (game_won(gr)) {
      game_delete(gr);
      return 0;
    }
    game_delete(gr);
  }

  // test not expected tents
  game gr = game_random(10, 10, true, 6, 4, false);
  for (int i = 0; i < game_nb_rows(gr); i++) {
    for (int j = 0; j < game_nb_cols(gr); j++) {
      if (game_get_square(gr, i, j) == TENT) {
        game_delete(gr);
        return 0;
      }
    }
  }
  game_delete(gr);

  // test infinity loop
  game grloop = game_random(3, 3, true, 100, 0, true);
  if (grloop == NULL) {
    return 0;
  }

  int counter = 0;
  for (int i = 0; i < game_nb_rows(grloop); i++) {
    for (int j = 0; j < game_nb_cols(grloop); j++) {
      if (game_get_square(grloop, i, j) == TENT) {
        counter++;
      }
    }
  }
  if (counter <= 0 || counter > 100) {
    game_delete(grloop);
    return 0;
  }
  game_delete(grloop);

  return 1;
}

bool test_game_solve()
{
  // empty game test
  game emptgame = game_new_empty();
  game_solve(emptgame);
  for (int i = 0; i < game_nb_rows(emptgame); i++) {
    for (int j = 0; j < game_nb_cols(emptgame); j++) {
      if (game_get_square(emptgame, i, j) != EMPTY) {
        game_delete(emptgame);
        return 0;
      }
    }
  }
  game_delete(emptgame);

  // empty game ext test
  game emptgameext = game_new_empty_ext(8, 5, 1);
  game_solve(emptgameext);
  for (int i = 0; i < game_nb_rows(emptgameext); i++) {
    for (int j = 0; j < game_nb_cols(emptgameext); j++) {
      if (game_get_square(emptgameext, i, j) != EMPTY) {
        game_delete(emptgameext);
        return 0;
      }
    }
  }
  game_delete(emptgameext);


  // default game
  game defgame = game_default();
  game_solve(defgame);
  if (!game_won(defgame)) {
    game_delete(defgame);
      return 0;
    }
  game_delete(defgame);
  return 1;
}

int main(int argc, char* argv[])
{
  bool ok = 0;
  if (argc == 1) {
    fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  if (strcmp("dummy", argv[1]) == 0) {
    ok = test_dummy();
  }
  if (strcmp("game_set_square", argv[1]) == 0) {
    ok = test_game_set_square();
  }
  if (strcmp("game_set_expected_nb_tents_row", argv[1]) == 0) {
    ok = test_game_set_expected_nb_tents_row();
  }
  if (strcmp("game_set_expected_nb_tents_col", argv[1]) == 0) {
    ok = test_game_set_expected_nb_tents_col();
  }
  if (strcmp("game_get_square", argv[1]) == 0) {
    ok = test_game_get_square();
  }
  if (strcmp("game_get_expected_nb_tents_row", argv[1]) == 0) {
    ok = test_game_get_expected_nb_tents_row();
  }
  if (strcmp("game_get_expected_nb_tents_col", argv[1]) == 0) {
    ok = test_game_get_expected_nb_tents_col();
  }
  if (strcmp("game_get_expected_nb_tents_all", argv[1]) == 0) {
    ok = test_game_get_expected_nb_tents_all();
  }
  if (strcmp("game_get_current_nb_tents_row", argv[1]) == 0) {
    ok = test_game_get_current_nb_tents_row();
  }
  if (strcmp("game_get_current_nb_tents_col", argv[1]) == 0) {
    ok = test_game_get_current_nb_tents_col();
  }
  if (strcmp("game_get_current_nb_tents_all", argv[1]) == 0) {
    ok = test_game_get_current_nb_tents_all();
  }
  if (strcmp("game_new_ext", argv[1]) == 0) {
    ok = test_game_new_ext();
  }
  if (strcmp("game_new_empty_ext", argv[1]) == 0) {
    ok = test_game_new_empty_ext();
  }
  if (strcmp("game_nb_rows", argv[1]) == 0) {
    ok = test_game_nb_rows();
  }
  if (strcmp("game_nb_cols", argv[1]) == 0) {
    ok = test_game_nb_cols();
  }
  if (strcmp("game_is_wrapping", argv[1]) == 0) {
    ok = test_game_is_wrapping();
  }
  if (strcmp("game_undo", argv[1]) == 0) {
    ok = test_game_undo();
  }
  if (strcmp("game_redo", argv[1]) == 0) {
    ok = test_game_redo();
  }
  if (strcmp("game_random", argv[1]) == 0) {
    ok = test_game_random();
  }
  if (strcmp("game_solve", argv[1]) == 0) {
    ok = test_game_solve();
  }
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}