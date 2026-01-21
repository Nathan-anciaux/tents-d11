#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

bool test_dummy(void) {
  printf("Test dummy\n");
  return true;
}

bool test_game_new(void) {
  // Créer un plateau de default size avec des arbres
  square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                                                 EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                                                 EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                                                 EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};

  uint nb_tents_row[DEFAULT_SIZE] = {1, 2, 1, 2, 2, 0, 1, 3};
  uint nb_tents_col[DEFAULT_SIZE] = {3, 0, 2, 1, 2, 1, 2, 1};

  game g = game_new(squares, nb_tents_row, nb_tents_col);
  bool ok = true;

  // Vérifie si les le nombres de tentes dans le collones et les rows est
  // correct
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_row(g, i) != nb_tents_row[i] || game_get_expected_nb_tents_col(g, i) != nb_tents_col[i]) ok = false;
  }
  // On vérifie que toutes les cases non TREE sont bien EMPTY
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != TREE && game_get_square(g, i, j) != EMPTY) ok = false;
    }
  }

  // Vérifie quelques cases pour voir si elles sont bien attribuées
  if (game_get_square(g, 0, 0) != TREE || game_get_square(g, 7, 5) != TREE) ok = false;

  game_delete(g);
  return ok;
}

bool test_game_new_empty(void) {
  game g = game_new_empty();
  bool ok = true;

  // Vérifie si les le nombres de tentes dans le collones et les rows est
  // correct(bien de 0
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    if (game_get_expected_nb_tents_row(g, i) != 0 || game_get_expected_nb_tents_col(g, i) != 0) ok = false;
    // Vérifie que tout est bien EMPTY
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) != EMPTY) ok = false;
    }
  }
  // Vérifie quelques cases pour voir si elles sont bien attribuées(sécurité en
  // plus)
  game_set_square(g, 3, 3, TREE);
  game_fill_grass(g);
  if (game_get_square(g, 3, 3) != TREE) ok = false;

  game_delete(g);
  return ok;
}

bool test_game_copy(void) {
  game g = game_new_empty();
  game h = game_copy(g);
  bool ok = true;
  // On vérifie que une game est bien égale a elle même
  if (!game_equal(g, g)) ok = false;

  for (uint i = 0; i < 8; i++) {
    if (game_get_expected_nb_tents_col(h, i) != game_get_expected_nb_tents_col(g, i)) ok = false;
  }
  for (uint i = 0; i < 8; i++) {
    if (game_get_expected_nb_tents_row(h, i) != game_get_expected_nb_tents_row(g, i)) ok = false;
  }
  for (uint i = 0; i < 8; i++) {
    for (uint j = 0; j < 8; j++) {
      if (game_get_square(h, i, j) != game_get_square(g, i, j)) ok = false;
    }
  }

  // On vérifie ici que si on change une case de g cela ne modifia pas j et que
  // h et g ne sont donc pas liés
  game_set_square(g, 0, 0, TREE);
  if (game_get_square(h, 0, 0) == TREE) ok = false;

  game j = game_default();
  game l = game_copy(j);

  for (uint i = 0; i < 8; i++) {
    if (game_get_expected_nb_tents_col(l, i) != game_get_expected_nb_tents_col(j, i)) ok = false;
  }
  for (uint i = 0; i < 8; i++) {
    if (game_get_expected_nb_tents_row(l, i) != game_get_expected_nb_tents_row(j, i)) ok = false;
  }
  for (uint i = 0; i < 8; i++) {
    for (uint p = 0; p < 8; p++) {
      if (game_get_square(l, i, p) != game_get_square(j, i, p)) ok = false;
    }
  }

  // On vérifie que chaque case du plateau est identique
  for (uint i = 0; i < 8; i++) {
    for (uint p = 0; p < 8; p++) {
      square square1 = game_get_square(j, i, p);
      square square2 = game_get_square(l, i, p);
      if (square1 != square2) ok = false;
    }
  }

  // On vérifie ici que si on change une case de j cela ne modifia pas l et que
  // j et l ne sont donc pas liés
  game_set_square(j, 0, 0, EMPTY);
  if (game_get_square(l, 0, 0) != TREE) ok = false;

  // On vérifie que la copie du jeu est indépendante donc pas de liason mais
  // cette fois si sur un expected et pas une case
  game_set_expected_nb_tents_row(j, 0, 99);
  if (game_get_expected_nb_tents_row(l, 0) == 99) ok = false;

  // On vérifie maintenant les expected col
  game_set_expected_nb_tents_col(j, 3, 42);
  if (game_get_expected_nb_tents_col(l, 3) == 42) ok = false;

  game m = game_default();
  game n = game_copy(m);

  // On vérifie que les deux jeux sont égaux avant la modif
  if (!game_equal(m, n)) ok = false;

  // On modifie une case et vérifie la que les deux ne sont pas liées
  game_set_square(m, 0, 0, EMPTY);
  if (game_get_square(n, 0, 0) == EMPTY) ok = false;

  // On teste maintenant avec game_new_ext et game_new_empty_ext
  game x = game_new_empty_ext(6, 3, true);
  game y = game_copy(x);
  if (game_is_wrapping(x) != game_is_wrapping(y)) ok = false;
  // On vérifie que une game est bien égale a elle même
  if (!game_equal(x, x)) ok = false;

  for (uint i = 0; i < 3; i++) {
    if (game_get_expected_nb_tents_col(y, i) != game_get_expected_nb_tents_col(x, i)) ok = false;
  }
  for (uint i = 0; i < 6; i++) {
    if (game_get_expected_nb_tents_row(y, i) != game_get_expected_nb_tents_row(x, i)) ok = false;
  }
  for (uint i = 0; i < 6; i++) {
    for (uint j = 0; j < 3; j++) {
      if (game_get_square(y, i, j) != game_get_square(x, i, j)) ok = false;
    }
  }

  // On vérifie ici que si on change une case de g cela ne modifia pas j et que
  // h et g ne sont donc pas liés
  game_set_square(x, 0, 0, TREE);
  if (game_get_square(y, 0, 0) == TREE) ok = false;

  game_delete(g);
  game_delete(h);
  game_delete(j);
  game_delete(l);
  game_delete(m);
  game_delete(n);
  game_delete(x);
  game_delete(y);
  return ok;
}

bool test_game_equal(void) {
  // On copie un tableau(game vide)
  game g = game_new_empty();
  game h = game_copy(g);
  bool ok = true;

  // On vérifie que c'est égal ensuite on change dans h une case EMPTY par TREE
  // et on reverifie si c'est equal(cela ne l'est plus)
  if (!game_equal(g, h)) ok = false;
  game_set_square(h, 0, 0, TREE);
  if (game_equal(g, h)) ok = false;

  game_set_square(h, 1, 1, TENT);
  if (game_equal(g, h)) ok = false;
  game_set_square(h, 2, 2, GRASS);
  if (game_equal(g, h)) ok = false;

  // On fait la meme chose avec un empty_ext
  game x = game_new_empty_ext(6, 3, true);
  game y = game_copy(x);

  if (!game_equal(x, y)) ok = false;
  game_set_square(y, 0, 0, TREE);
  if (game_equal(x, y)) ok = false;

  game_set_square(y, 1, 1, TENT);
  if (game_equal(x, y)) ok = false;
  game_set_square(h, 2, 2, GRASS);
  if (game_equal(x, y)) ok = false;

  game_delete(g);
  game_delete(h);
  game_delete(x);
  game_delete(y);
  return ok;
}

bool test_game_delete(void) {
  game g = game_new_empty();
  bool ok = true;
  game_delete(g);
  game x = game_new_empty();
  game_delete(x);
  return ok;
}

bool test_game_fill_grass(void) {
  game g = game_new_empty();
  bool ok = true;

  // On place des cases prédéfinie pour tester tous les cas
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 1, 1, TENT);
  game_set_square(g, 2, 2, GRASS);
  game_set_square(g, DEFAULT_SIZE - 1, DEFAULT_SIZE - 1, EMPTY);
  game_fill_grass(g);

  // On vérifie que toutes les cases EMPTY sont devenues GRASS
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      square sq = game_get_square(g, i, j);
      if ((i == 0 && j == 0) || (i == 1 && j == 1)) {
        if ((i == 0 && j == 0 && sq != TREE) || (i == 1 && j == 1 && sq != TENT)) ok = false;
      } else if (i == 2 && j == 2) {
        // On vérifie que GRASS est resté GRASS
        if (sq != GRASS) ok = false;
      } else {
        // Toutes les autres doivent être GRASS
        if (sq != GRASS) ok = false;
      }
    }
  }

  // On Vérifie que les bords sont bien remplis
  if (game_get_square(g, DEFAULT_SIZE - 1, DEFAULT_SIZE - 1) != GRASS) ok = false;

  // Même chose avec du game_new_empty_ext
  game x = game_new_empty_ext(6, 3, true);

  // On place des cases prédéfinie pour tester tous les cas
  game_set_square(x, 0, 0, TREE);
  game_set_square(x, 1, 1, TENT);
  game_set_square(x, 2, 2, GRASS);
  game_set_square(x, 6 - 1, 3 - 1, EMPTY);
  game_fill_grass(x);

  // On vérif si on est toujours bien en wrapping
  if (!game_is_wrapping(x)) ok = false;

  // On vérifie que toutes les cases EMPTY sont devenues GRASS
  for (uint i = 0; i < 6; i++) {
    for (uint j = 0; j < 3; j++) {
      square sq = game_get_square(x, i, j);
      if ((i == 0 && j == 0) || (i == 1 && j == 1)) {
        if ((i == 0 && j == 0 && sq != TREE) || (i == 1 && j == 1 && sq != TENT)) ok = false;
      } else if (i == 2 && j == 2) {
        // On vérifie que GRASS est resté GRASS
        if (sq != GRASS) ok = false;
      } else {
        // Toutes les autres doivent être GRASS
        if (sq != GRASS) ok = false;
      }
    }
  }

  // On Vérifie que les bords sont bien remplis
  if (game_get_square(x, 6 - 1, 3 - 1) != GRASS) ok = false;

  game_delete(g);
  game_delete(x);
  return ok;
}

bool test_game_default(void) {
  game g = game_default();
  bool ok = true;

  if (game_get_expected_nb_tents_col(g, 0) != 3 || game_get_expected_nb_tents_col(g, 1) != 0 || game_get_expected_nb_tents_col(g, 2) != 2 ||
      game_get_expected_nb_tents_col(g, 3) != 1 || game_get_expected_nb_tents_col(g, 4) != 2 || game_get_expected_nb_tents_col(g, 5) != 1 ||
      game_get_expected_nb_tents_col(g, 6) != 2 || game_get_expected_nb_tents_col(g, 7) != 1 || game_get_expected_nb_tents_row(g, 0) != 1 ||
      game_get_expected_nb_tents_row(g, 1) != 2 || game_get_expected_nb_tents_row(g, 2) != 1 || game_get_expected_nb_tents_row(g, 3) != 2 ||
      game_get_expected_nb_tents_row(g, 4) != 2 || game_get_expected_nb_tents_row(g, 5) != 0 || game_get_expected_nb_tents_row(g, 6) != 1 ||
      game_get_expected_nb_tents_row(g, 7) != 3)
    ok = false;

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      bool abres = (i == 0 && j == 0) || (i == 0 && j == 5) || (i == 0 && j == 6) || (i == 2 && j == 6) || (i == 3 && j == 0) || (i == 3 && j == 2) || (i == 4 && j == 4) ||
                   (i == 5 && j == 2) || (i == 7 && j == 1) || (i == 7 && j == 2) || (i == 7 && j == 5) || (i == 7 && j == 6);

      square square = game_get_square(g, i, j);

      if (abres) {
        if (square != TREE) ok = false;
      } else {
        if (square != EMPTY && square != GRASS && square != TENT) ok = false;
      }
    }
  }
  game_restart(g);

  // On vérifie que les arbres sont toujours là
  uint trees[12][2] = {{0, 0}, {0, 5}, {0, 6}, {2, 6}, {3, 0}, {3, 2}, {4, 4}, {5, 2}, {7, 1}, {7, 2}, {7, 5}, {7, 6}};

  for (uint t = 0; t < 12; t++) {
    uint i = trees[t][0];
    uint j = trees[t][1];
    if (game_get_square(g, i, j) != TREE) ok = false;
  }

  // On vérifie que les autres cases sont bien redevenues EMPTY
  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    for (uint j = 0; j < DEFAULT_SIZE; j++) {
      bool is_tree = false;
      for (uint t = 0; t < 12; t++) {
        if (trees[t][0] == i && trees[t][1] == j) {
          is_tree = true;
          break;
        }
      }
      if (!is_tree && game_get_square(g, i, j) != EMPTY) ok = false;
    }
  }

  game_delete(g);
  return ok;
}

bool test_game_is_neigh(void) {
  game g = game_new_empty();
  bool ok = true;

  // On place un arbre
  game_set_square(g, 3, 3, TREE);

  // On vérifie si les cases autour le détecte(sans diagonales)
  if (!game_is_neigh(g, 2, 3, TREE, false) || !game_is_neigh(g, 3, 2, TREE, false) || !game_is_neigh(g, 3, 4, TREE, false) || !game_is_neigh(g, 4, 3, TREE, false)) ok = false;

  // On vérifie maintenant la même chose avec les diagonales activées
  if (!game_is_neigh(g, 2, 2, TREE, true) || !game_is_neigh(g, 2, 4, TREE, true) || !game_is_neigh(g, 4, 2, TREE, true) || !game_is_neigh(g, 4, 4, TREE, true) ||
      !game_is_neigh(g, 2, 3, TREE, true) || !game_is_neigh(g, 3, 2, TREE, true) || !game_is_neigh(g, 3, 4, TREE, true) || !game_is_neigh(g, 4, 3, TREE, true))
    ok = false;

  // On vérifie que des cases qui ne sont pas a coté ne le détecte pas
  if (game_is_neigh(g, 3, 7, TREE, false) || game_is_neigh(g, 6, 7, TREE, false)) ok = false;

  // On vérifie aussi si le fait qu'on active pas les diagonals ne détectent
  // donc pas le TREE qui est en diagonale
  if (game_is_neigh(g, 0, 0, TREE, false)) ok = false;

  // On vérifie les bordures
  game_set_square(g, 0, 0, TREE);
  if (!game_is_neigh(g, 0, 1, TREE, false)) ok = false;

  // On vérifie sans diagonale que aucun voisin ne soit détecter en dehors de la
  // grille
  if (game_is_neigh(g, 0, 7, TREE, false) || game_is_neigh(g, 7, 0, TREE, false)) ok = false;

  // On vérifie avec diagonales que seule la diagonale (1,1) est détecté et donc
  // qu'on ne vas pas en dehors du tableau
  if (!game_is_neigh(g, 1, 1, TREE, true)) ok = false;

  // On teste maintenant une game ext avec un wraping
  game x = game_new_empty_ext(3, 5, true);
  game_set_square(x, 0, 5 - 1, TREE);
  game_set_square(x, 0, 3, BIGTREE);
  game_set_square(x, 2, 0, BIGTREE);
  game_set_square(x, 3 - 1, 5 - 1, GRASS);

  if (!game_is_neigh(x, 0, 0, TREE, false) || !game_is_neigh(x, 0, 0, BIGTREE, false)) ok = false;
  if (!game_is_neigh(x, 0, 0, EMPTY, false) || !game_is_neigh(x, 0, 0, EMPTY, false)) ok = false;
  if (!game_is_neigh(x, 0, 0, EMPTY, true) || !game_is_neigh(x, 0, 0, GRASS, true)) ok = false;

  game_delete(g);
  game_delete(x);
  return ok;
}

bool test_game_new_ext(void) {
  uint rows = 3;
  uint cols = 5;
  bool wrapping = true;

  square squares[] = {TREE, EMPTY, EMPTY, BIGTREE, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY, BIGTREE, EMPTY, EMPTY, EMPTY, TREE};

  uint expected_row[] = {1, 1, 2};
  uint expected_col[] = {1, 0, 1, 1, 1};
  game g = game_new_ext(rows, cols, squares, expected_row, expected_col, wrapping);
  bool ok = true;
  // On teste les dimensions de la grille
  if (game_nb_rows(g) != rows || game_nb_cols(g) != cols) ok = false;

  // On test si on est bien en wrapping
  if (!game_is_wrapping(g)) ok = false;

  // Vérifie si les le nombres de tentes dans le collones et les rows est correct
  for (uint i = 0; i < cols; i++) {
    if (game_get_expected_nb_tents_col(g, i) != expected_col[i]) ok = false;
  }
  for (uint i = 0; i < rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != expected_row[i]) ok = false;
  }
  // On vérifie que toutes les cases non TREE sont bien EMPTY
  for (uint i = 0; i < rows; i++) {
    for (uint j = 0; j < cols; j++) {
      if (game_get_square(g, i, j) != TREE && game_get_square(g, i, j) != EMPTY && game_get_square(g, i, j) != BIGTREE) ok = false;
      square v = game_get_square(g, i, j);
      if (v != squares[i * cols + j]) ok = false;
    }
  }

  // Vérifie quelques cases pour voir si elles sont bien attribuées
  if (game_get_square(g, 0, 0) != TREE || game_get_square(g, 0, 3) != BIGTREE) ok = false;

  game_delete(g);
  return ok;
}

bool test_game_new_empty_ext(void) {
  uint rows = 4;
  uint cols = 6;
  bool wrapping = true;

  game g = game_new_empty_ext(rows, cols, wrapping);
  bool ok = true;

  if (game_nb_rows(g) != rows) ok = false;
  if (game_nb_cols(g) != cols) ok = false;

  if (game_is_wrapping(g) != wrapping) ok = false;

  for (uint i = 0; i < rows; i++) {
    for (uint j = 0; j < cols; j++) {
      square s = game_get_square(g, i, j);
      if (s != EMPTY) ok = false;
    }
  }

  for (uint i = 0; i < rows; i++) {
    if (game_get_expected_nb_tents_row(g, i) != 0) ok = false;
  }
  for (uint j = 0; j < cols; j++) {
    if (game_get_expected_nb_tents_col(g, j) != 0) ok = false;
  }

  game_delete(g);
  return ok;
}

bool test_game_nb_neigh(void) {
  game g = game_new_empty_ext(3, 5, true);
  bool ok = true;
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 0, 4, GRASS);
  game_set_square(g, 1, 1, GRASS);
  game_set_square(g, 2, 0, BIGTREE);
  game_set_square(g, 2, 4, TREE);

  // On vérifie d'abord avec du wrapping et sans diag
  if (game_nb_neigh(g, 0, 0, EMPTY, false) != 2 || game_nb_neigh(g, 0, 0, GRASS, false) != 1 || game_nb_neigh(g, 0, 0, BIGTREE, false) != 1 ||
      game_nb_neigh(g, 0, 0, TREE, false) != 0)
    ok = false;

  // maintenant avec du wrapping et avec diag
  if (game_nb_neigh(g, 0, 0, EMPTY, true) != 4 || game_nb_neigh(g, 0, 0, GRASS, true) != 2 || game_nb_neigh(g, 0, 0, BIGTREE, true) != 1 || game_nb_neigh(g, 0, 0, TREE, true) != 1)
    ok = false;

  game x = game_new_empty();

  game_set_square(x, 0, 1, TREE);
  game_set_square(x, 0, 2, GRASS);
  game_set_square(x, 0, 3, BIGTREE);
  game_set_square(x, 1, 3, TREE);
  game_set_square(x, 2, 3, GRASS);
  game_set_square(x, 2, 1, BIGTREE);

  // On vérifie avec une game classique sans wrapping et ici sans diag
  if (game_nb_neigh(x, 1, 2, EMPTY, false) != 2 || game_nb_neigh(x, 1, 2, GRASS, false) != 1 || game_nb_neigh(x, 1, 2, BIGTREE, false) != 0 ||
      game_nb_neigh(x, 1, 2, TREE, false) != 1)
    ok = false;
  // Maintenant sans wrapping mais diag en game classique
  if (game_nb_neigh(x, 1, 2, EMPTY, true) != 2 || game_nb_neigh(x, 1, 2, GRASS, true) != 2 || game_nb_neigh(x, 1, 2, BIGTREE, true) != 2 || game_nb_neigh(x, 1, 2, TREE, true) != 2)
    ok = false;

  game_delete(g);
  game_delete(x);
  return ok;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Erreur: un seul argument attendu.\n");
    return EXIT_FAILURE;
  }
  fprintf(stderr,
          "=> Start test \"%s"
          "\n",
          argv[1]);
  bool ok = false;

  if (strcmp(argv[1], "dummy") == 0)
    ok = test_dummy();
  else if (strcmp(argv[1], "game_new") == 0)
    ok = test_game_new();
  else if (strcmp(argv[1], "game_new_empty") == 0)
    ok = test_game_new_empty();
  else if (strcmp(argv[1], "game_copy") == 0)
    ok = test_game_copy();
  else if (strcmp(argv[1], "game_equal") == 0)
    ok = test_game_equal();
  else if (strcmp(argv[1], "game_delete") == 0)
    ok = test_game_delete();
  else if (strcmp(argv[1], "game_fill_grass") == 0)
    ok = test_game_fill_grass();
  else if (strcmp(argv[1], "game_default") == 0)
    ok = test_game_default();
  else if (strcmp(argv[1], "game_is_neigh") == 0)
    ok = test_game_is_neigh();
  else if (strcmp(argv[1], "game_new_ext") == 0)
    ok = test_game_new_ext();
  else if (strcmp(argv[1], "game_new_empty_ext") == 0)
    ok = test_game_new_empty_ext();
  else if (strcmp(argv[1], "game_nb_neigh") == 0)
    ok = test_game_nb_neigh();
  else {
    fprintf(stderr, "Erreur: test \"%s\" introuvable.\n", argv[1]);
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
