#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "queue.h"

bool test_dummy(void) { return true; }
bool test_game_set_expected_nb_tents_row(void) {
  game g = game_default();
  bool ok = true;

  if (game_get_expected_nb_tents_row(g, 0) != 1) ok = false;
  if (game_get_expected_nb_tents_row(g, 1) != 2) ok = false;

  game_set_expected_nb_tents_row(g, 0, 3);
  if (game_get_expected_nb_tents_row(g, 0) != 3) ok = false;

  game_set_expected_nb_tents_row(g, 7, 5);
  if (game_get_expected_nb_tents_row(g, 7) != 5) ok = false;
  game h = game_new_empty_ext(8, 8, false);
  for (uint i = 0; i < 8; i++) {
    game_set_expected_nb_tents_row(h, i, 3);
    if (game_get_expected_nb_tents_row(h, i) != 3) ok = false;
  }
  game_delete(h);
  game_delete(g);
  return ok;
}
bool test_game_set_expected_nb_tents_col(void) {
  game g = game_default();
  bool ok = true;
  if (game_get_expected_nb_tents_col(g, 0) != 3) ok = false;
  if (game_get_expected_nb_tents_col(g, 1) != 0) ok = false;

  game_set_expected_nb_tents_col(g, 0, 4);
  if (game_get_expected_nb_tents_col(g, 0) != 4) ok = false;

  game_set_expected_nb_tents_col(g, 7, 2);
  if (game_get_expected_nb_tents_col(g, 7) != 2) ok = false;
  game h = game_new_empty_ext(8, 8, false);
  for (uint i = 0; i < 8; i++) {
    game_set_expected_nb_tents_col(h, i, 3);
    if (game_get_expected_nb_tents_col(h, i) != 3) ok = false;
  }
  game_delete(h);
  game_delete(g);
  return ok;
}
bool test_game_get_expected_nb_tents_row(void) {
  game g = game_default();
  bool ok = true;
  if (game_get_expected_nb_tents_row(g, 0) != 1) ok = false;
  if (game_get_expected_nb_tents_row(g, 1) != 2) ok = false;
  game_set_expected_nb_tents_row(g, 3, 4);
  if (game_get_expected_nb_tents_row(g, 3) != 4) ok = false;
  game h = game_new_empty_ext(8, 8, false);
  for (uint i = 0; i < 8; i++) {
    game_set_expected_nb_tents_row(h, i, 3);
    if (game_get_expected_nb_tents_row(h, i) != 3) ok = false;
  }
  game_delete(h);
  game_delete(g);
  return ok;
}

bool test_game_get_expected_nb_tents_col(void) {
  game g = game_default();
  bool ok = true;
  if (game_get_expected_nb_tents_col(g, 0) != 3) ok = false;
  if (game_get_expected_nb_tents_col(g, 1) != 0) ok = false;

  game_set_expected_nb_tents_col(g, 2, 5);
  if (game_get_expected_nb_tents_col(g, 2) != 5) ok = false;

  game h = game_new_empty_ext(8, 8, false);
  for (uint i = 0; i < 8; i++) {
    game_set_expected_nb_tents_col(h, i, 3);
    if (game_get_expected_nb_tents_col(h, i) != 3) ok = false;
  }
  game_delete(h);
  game_delete(g);
  return ok;
}
bool test_game_get_expected_nb_tents_all(void) {
  game g = game_default();
  bool ok = true;
  uint expected = 12;
  if (game_get_expected_nb_tents_all(g) != expected) ok = false;
  game_delete(g);

  game g2 = game_default_ext();
  expected = 4;
  if (game_get_expected_nb_tents_all(g2) != expected) ok = false;
  game_delete(g2);

  game g4 = game_new_empty_ext(5, 6, false);
  if (game_get_expected_nb_tents_all(g4) != 0) ok = false;
  game_delete(g4);
  return ok;
}

bool test_game_restart(void) {
  game g = game_default();
  game p = game_default();
  bool ok = true;

  game_set_square(g, 1, 1, GRASS);
  game_set_square(g, 1, 3, TENT);
  game_set_square(g, 7, 7, TENT);

  game_restart(g);

  for (uint i = 0; i < 8; i++) {
    for (uint j = 0; j < 8; j++) {
      if (game_get_square(g, i, j) != game_get_square(p, i, j)) ok = false;
      if (game_get_expected_nb_tents_row(g, i) != game_get_expected_nb_tents_row(p, i) || game_get_expected_nb_tents_col(g, i) != game_get_expected_nb_tents_col(p, i) ||
          game_get_expected_nb_tents_all(g) != game_get_expected_nb_tents_all(p))
        ok = false;
    }
  }
  game_delete(g);
  game_delete(p);
  return ok;
}
bool test_game_print(void) {
  game g = game_default();
  game_print(g);
  printf("\n game print sans encombres\n");
  game_delete(g);
  return true;
}
bool test_game_default_solution(void) {
  game g = game_default_solution();
  bool ok = true;

  for (uint i = 0; i < 8; i++) {
    for (uint j = 0; j < 8; j++) {
      if (game_get_square(g, i, j) == EMPTY) ok = false;
    }
  }

  if (!game_won(g)) ok = false;
  game_delete(g);
  return ok;
}

bool test_game_undo(void) {
  game g = game_default();
  game g2 = game_default();
  // game f = game_new_empty_ext(4, 5, true);
  bool ok = true;

  game_play_move(g, 0, 1, TENT);
  game_play_move(g, 1, 1, TENT);
  game_play_move(g, 1, 1, GRASS);

  game_undo(g);
  if (game_get_square(g, 1, 1) != TENT) ok = false;
  game_undo(g);
  if (game_get_square(g, 1, 1) != EMPTY) ok = false;
  game_undo(g);
  if (game_get_square(g, 0, 1) != EMPTY) ok = false;
  if (!game_equal(g, g2)) ok = false;
  game_delete(g2);

  game_delete(g);

  game g3 = game_default();
  game g4 = game_default();
  game_play_move(g3, 0, 1, TENT);
  game_play_move(g3, 1, 1, TENT);

  game_restart(g3);
  game_undo(g3);

  if (!game_equal(g3, g4)) ok = false;
  game_delete(g3);
  game_delete(g4);

  return ok;
}

bool test_game_redo(void) {  // Modifié par Maxence car tes tests étaitent faibles ( ça testais presque rien ) et en plus c'étaitait faux
  game g = game_default();
  game g2 = game_default();
  game f = game_new_empty_ext(4, 5, true);
  bool ok = true;

  game_play_move(g, 0, 1, TENT);
  game_play_move(g, 1, 1, TENT);
  game_play_move(g, 1, 1, GRASS);

  game_play_move(g2, 0, 1, TENT);
  game_play_move(g2, 1, 1, TENT);
  game_play_move(g2, 1, 1, GRASS);

  game_undo(g);
  game_undo(g);
  game_undo(g);

  game_redo(g);
  if (game_get_square(g, 0, 1) != TENT) ok = false;
  game_redo(g);
  if (game_get_square(g, 1, 1) != TENT) ok = false;
  game_redo(g);
  if (game_get_square(g, 1, 1) != GRASS) ok = false;
  game_redo(g);
  if (!game_equal(g, g2)) ok = false;
  game_delete(g);
  game_delete(g2);

  game_fill_grass(f);
  game_undo(f);
  game_redo(f);
  for (uint i = 0; i < 4; i++) {
    for (uint j = 0; j < 5; j++) {
      if (game_get_square(f, i, j) != GRASS) {
        ok = false;
      }
    }
  }
  game_delete(f);

  game g3 = game_default();
  game g4 = game_default();
  game_play_move(g3, 0, 1, TENT);
  game_play_move(g3, 1, 1, TENT);
  game_undo(g3);
  game_restart(g3);
  game_redo(g3);

  if (!game_equal(g3, g4)) ok = false;
  game_delete(g3);
  game_delete(g4);

  return ok;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Erreur un seul argument attendu.\n");
    return false;
  }
  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;
  if (strcmp(argv[1], "dummy") == 0)
    ok = test_dummy();
  else if (strcmp("game_set_expected_nb_tents_row", argv[1]) == 0)
    ok = test_game_set_expected_nb_tents_row();
  else if (strcmp("game_set_expected_nb_tents_col", argv[1]) == 0)
    ok = test_game_set_expected_nb_tents_col();
  else if (strcmp("game_get_expected_nb_tents_row", argv[1]) == 0)
    ok = test_game_get_expected_nb_tents_row();
  else if (strcmp("game_get_expected_nb_tents_col", argv[1]) == 0)
    ok = test_game_get_expected_nb_tents_col();
  else if (strcmp("game_get_expected_nb_tents_all", argv[1]) == 0)
    ok = test_game_get_expected_nb_tents_all();
  else if (strcmp("game_restart", argv[1]) == 0)
    ok = test_game_restart();
  else if (strcmp("game_print", argv[1]) == 0)
    ok = test_game_print();
  else if (strcmp("game_default_solution", argv[1]) == 0)
    ok = test_game_default_solution();
  /*else if (strcmp("game_default_solution_ext", argv[1]) == 0)
    ok = test_game_default_solution_ext();*/
  else if (strcmp("game_undo", argv[1]) == 0)
    ok = test_game_undo();
  else if (strcmp("game_redo", argv[1]) == 0)
    ok = test_game_redo();
  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    return EXIT_FAILURE;
  }
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}
