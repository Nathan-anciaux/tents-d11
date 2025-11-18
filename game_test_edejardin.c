#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"

bool test_game_new(void);
bool test_game_new_empty(void);
bool test_game_copy(void);
bool test_game_equal(void);
bool test_game_delete(void);
bool test_game_play_move(void);
bool test_game_check_move(void);
int test_dummy(void);

int test_dummy() { return 1; }

// ici je teste si les initialisationd e game_new sont pas deconnantes
bool test_game_new(void)
{
  // on initialise le game_default en gros
  uint tents_row[DEFAULT_SIZE] = {1, 2, 1, 2, 2, 0, 1, 3};
  uint tents_col[DEFAULT_SIZE] = {3, 0, 2, 1, 2, 1, 2, 1};
  square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
      EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};

  game g_new = game_new(squares, tents_row, tents_col);
  game g_ref = game_default();
  if (g_new == NULL || g_ref == NULL) return 0;
  if (!game_equal(g_new, g_ref)) return 0;
  game_delete(g_ref);

  // verifier si on peut enlever ça normalement oui
  squares[0] = EMPTY;
  tents_row[0] = 99;
  tents_col[0] = 99;
  if (game_get_square(g_new, 0, 0) != TREE) return 0;
  if (game_get_expected_nb_tents_row(g_new, 0) != 1) return 0;
  if (game_get_expected_nb_tents_col(g_new, 0) != 3) return 0;
  tents_row[0] = 1;
  tents_col[0] = 3;

  // on regarde si chaque ligne a le bon nombre de tentes
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    if (tents_col[i] != game_get_expected_nb_tents_col(g_new, i)) return 0;
    if (tents_row[i] != game_get_expected_nb_tents_row(g_new, i)) return 0;
  }
  game_delete(g_new);

  // initialisation expres pas possible donc pas le game default
  squares[0] = TREE;
  tents_row[0] = 7;
  tents_col[0] = 7;
  g_new = game_new(squares, tents_row, tents_col);
  g_ref = game_default();
  if (g_new == NULL || g_ref == NULL) return 0;
  if (game_equal(g_new, g_ref)) return 0;

  // Attention j'ai fait l'implementation naive qui va empecher les nombres abusés, mais faut faire nombre de empty /2
  // (+1) fin meme pas c'est plus subtil nsm
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    if (tents_col[i] != game_get_expected_nb_tents_col(g_new, i)) return 0;
    if (tents_row[i] != game_get_expected_nb_tents_row(g_new, i)) return 0;
  }

  game_delete(g_new);
  game_delete(g_ref);
  return 1;
}

bool test_game_new_empty(void)
{
  game g = game_new_empty();
  if (g == NULL) return 0;

  // on regarde si c'est bien empty partout
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != EMPTY) return 0;
    }
  }
  // on regarde si c'est bien 0 partout
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_col(g, i) != 0) return 0;
    if (game_get_expected_nb_tents_row(g, i) != 0) return 0;
  }
  if (game_get_current_nb_tents_all(g) != 0) return 0;

  game_delete(g);
  return 1;
}

/*static bool check_null(game g)
{
  if (g == NULL) {
    game_delete(g);
    return 1;
  }
  return 0;
}*/

// je prend chaque possibilités de copies possible et je fais des equals
bool test_game_copy()
{
  game g1 = game_default();
  game g1_copy = game_copy(g1);
  game g2 = game_new_empty();
  game g2_copy = game_copy(g2);
  game g3 = game_default_solution();
  game g3_copy = game_copy(g3);

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g1, i, j) != game_get_square(g1_copy, i, j) ||
          game_get_square(g2, i, j) != game_get_square(g2_copy, i, j) ||
          game_get_square(g3, i, j) != game_get_square(g3_copy, i, j))
        return 0;
      ;
    }
  }
  if (g1 == g1_copy) return 0;
  if (g3 == g3_copy) return 0;
  if (g2 == g2_copy) return 0;

  game_delete(g1);
  game_delete(g2);
  game_delete(g3);
  game_delete(g1_copy);
  game_delete(g2_copy);
  game_delete(g3_copy);
  return 1;
}

//
bool test_game_equal(void)
{
  game g1 = game_default();
  game g2 = game_default();
  if (g1 == NULL || g2 == NULL || !game_equal(g1, g2)) return 0;
  // on regarde l'egalité pour les nbr de tentes demandées
  if (game_get_current_nb_tents_all(g1) != game_get_current_nb_tents_all(g2)) return 0;
  if (game_get_expected_nb_tents_all(g1) != game_get_expected_nb_tents_all(g2)) return 0;

  game g_copy = game_copy(g1);
  if (g1 == NULL || !game_equal(g1, g_copy)) {game_delete(g_copy); return 0;}
  game_delete(g_copy);

  game g_fill = game_copy(g1);
  game_fill_grass(g_fill);
  game gf1 = game_copy(g_fill);
  if (!game_equal(gf1, g_fill)) return 0;

  int test[3] = {TENT, EMPTY};
  for (int i = 0; i < 2; i++) {
    game_play_move(g_fill, 0, 4, test[i]);
    if (game_equal(gf1, g_fill)) return 0;
  }

  game_delete(g_fill);
  game_delete(gf1);
  
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    game_set_expected_nb_tents_row(g2, i, 99);
    game_set_expected_nb_tents_col(g2, i, 99);
    if (game_equal(g2, g1)) return 0;
  }

  game_delete(g1);
  game_delete(g2);
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

  // le set de test pour chaque possibilité
  int test[9] = {EMPTY, TENT, TENT, EMPTY, GRASS, GRASS, TENT, GRASS, EMPTY};
  int row0, col1, all;

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
  int test_tents[12] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1};
  int test[15] = {TENT, GRASS, EMPTY, TREE, TREE, TREE, EMPTY, TENT, TENT, EMPTY, GRASS, GRASS, TENT, GRASS, EMPTY};
  // je fais les tests arbre sur x et x sur arbres
  for (int i = 0, j = 0; j < 6 && i < 6; i++, j = j + 2) {
    if (game_check_move(g1, test_tents[j], test_tents[j + 1], test[i])) return 0;
  }
  // je fais toutes les autres possibilités possibles
  for (int i = 6; i < 15; i++) {
    if (!game_check_move(g, 0, 4, test[i])) return 0;
  }
  game_delete(g);
  game_delete(g1);
  return 1;
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
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS \n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE \n", argv[1]);
    return EXIT_FAILURE;
  }
}