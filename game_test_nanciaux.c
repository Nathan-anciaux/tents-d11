#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

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

int test_dummy() { return 1; }

bool test_game_set_square() {
  // test sur game empty
  game g = game_new_empty();
  if (g == NULL) {
    return false;
  }
  int L[] = {TENT, GRASS, EMPTY, TREE};
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      for (int k = 0; k < 4; k++) {
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

bool test_game_set_expected_nb_tents_row() {
  // test sur game empty
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

bool test_game_set_expected_nb_tents_col() {
  // test sur game empty
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

bool test_game_get_square() {
  // test sur game empty et sur new game
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
  square squares[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
                      TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};
  game g = game_new(squares, nb_tents_row, nb_tents_col);
  if (g == NULL) {
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

bool test_game_get_expected_nb_tents_row() {
  // test sur game empty et sur new game
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
  square squares[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
                      TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};
  game g = game_new(squares, nb_tents_row, nb_tents_col);
  if (g == NULL) {
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

bool test_game_get_expected_nb_tents_col() {
  // test sur game empty et sur new game
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
  square squares[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
                      TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};
  game g = game_new(squares, nb_tents_row, nb_tents_col);
  if (g == NULL) {
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

bool test_game_get_expected_nb_tents_all() {
  // test sur game empty et sur new game et sur game default
  game g_empty = game_new_empty();
  game g = game_default();
  if (g == NULL || g_empty == NULL) {
    game_delete(g);
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
  square squares[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
                      TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                      EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};
  game g_new = game_new(squares, nb_tents_row, nb_tents_col);
  if (g_new == NULL) {
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

bool test_game_get_current_nb_tents_row() {
  // test sur game default
  game g = game_default();
  int s = 0;
  if (g == NULL) {
    game_delete(g);
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

bool test_game_get_current_nb_tents_col() {
  // test sur game default
  game g = game_default();
  int s = 0;
  if (g == NULL) {
    game_delete(g);
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
bool test_game_get_current_nb_tents_all() {
  // test sur game default et game empty
  game g = game_default();
  game g_empty = game_new_empty();
  int s = 0;
  if (g == NULL || g_empty == NULL) {
    game_delete(g);
    game_delete(g_empty);
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

bool test_game_new_ext() { return false; }

bool test_game_redo() { return false; }

bool test_game_undo() { return false; }

bool test_game_is_wrapping() { return false; }

bool test_game_nb_cols() { return false; }

bool test_game_nb_rows() { return false; }

bool test_game_new_empty_ext() { return false; }

int main(int argc, char* argv[]) {
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
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}