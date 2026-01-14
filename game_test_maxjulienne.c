#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"

game create_game_rectangle()
{
  // Game rectangle
  square squares[10] = {EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  uint nb_row[6] = {1, 0, 0, 0, 0, 0};
  uint nb_col[2] = {1, 0};
  game g = game_new_ext(5, 2, squares, nb_row, nb_col, true);
  return g;
}

// 1 = TRUE

int test_game_won()
{
  game g;
  int result;

  g = game_default_solution();
  if (game_won(g) != 1) {
    game_delete(g);
    return 0;
  }
  game_delete(g);
  // Sur jeu vide
  g = game_new_empty();
  result = game_won(g);
  game_delete(g);
  if (result != 1) {
    return 0;
  }

  // Cas de base
  g = game_default();
  result = game_won(g);
  game_delete(g);
  if (result != 0) {
    return 0;
  }

  // Tentes adjacentes : horizontale
  g = game_default();
  game_set_square(g, 1, 0, TENT);
  game_set_square(g, 1, 1, TENT);
  result = game_won(g);
  if (result != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  // Tentes adjacentes : Verticale
  g = game_default();
  game_set_square(g, 0, 1, TENT);
  game_set_square(g, 1, 1, TENT);
  result = game_won(g);
  if (result != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  // Tentes adjacentes : Diagonale
  g = game_default();
  game_set_square(g, 1, 1, TENT);
  game_set_square(g, 2, 2, TENT);
  result = game_won(g);
  if (result != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  // Règle 2
  g = game_default_solution();
  game_set_square(g, 1, 0, GRASS);
  game_set_square(g, 0, 1, TENT);
  result = game_won(g);
  if (result != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  // Règle 3 : nb tente != nb arbre
  g = game_default_solution();
  game_set_square(g, 1, 1, TENT);
  result = game_won(g);
  if (result != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  // Sans doute à revoir
  g = game_default_solution();
  game_set_square(g, 0, 4, EMPTY);
  result = game_won(g);
  if (result != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  g = game_default_solution();
  game_set_square(g, 0, 4, GRASS);
  game_set_square(g, 0, 2, TENT);
  result = game_won(g);
  if (result != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  g = game_default_solution();
  game_set_square(g, 1, 1, TENT);
  if (game_won(g) != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  g = game_default_solution();
  game_set_square(g, 0, 4, GRASS);
  game_set_square(g, 1, 1, TENT);
  if (game_won(g) != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  g = game_default_solution();
  game_set_square(g, 0, 4, GRASS);
  result = game_won(g);
  if (result != 0) {
    return 0;
  }
  game_delete(g);

  g = game_new_empty();
  game_set_square(g, 0, 4, TENT);
  result = game_won(g);
  if (result != 0) {
    return 0;
  }
  game_delete(g);

  // Test sur bigtree -> 2 tentes piles a coté d'un bigtree
  // Test normalement gagnant
  g = game_new_empty();
  game_set_square(g, 1, 1, BIGTREE);
  game_set_square(g, 0, 1, TENT);
  game_set_square(g, 2, 1, TENT);
  game_set_expected_nb_tents_col(g, 1, 2);
  game_set_expected_nb_tents_row(g, 2, 1);
  game_set_expected_nb_tents_row(g, 0, 1);

  game_fill_grass(g);
  result = game_won(g);
  if (result != 1) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  // Test normalement perdant
  g = game_new_empty();
  game_set_square(g, 1, 1, BIGTREE);
  game_set_square(g, 0, 1, TENT);
  game_set_expected_nb_tents_col(g, 1, 1);
  game_set_expected_nb_tents_row(g, 0, 1);
  game_fill_grass(g);
  result = game_won(g);
  if (result != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  // Test de wrapping
  // Test normalement gagnant
  g = game_new_empty_ext(8, 8, true);
  game_set_square(g, 0, 0, TREE);
  game_set_square(g, 0, 7, TENT);
  game_set_expected_nb_tents_col(g, 7, 1);
  game_set_expected_nb_tents_row(g, 0, 1);

  game_fill_grass(g);
  result = game_won(g);
  if (result != 1) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  // Test normalement perdant
  g = game_new_empty_ext(8, 8, true);
  game_set_square(g, 0, 7, TENT);
  game_set_expected_nb_tents_col(g, 7, 1);
  game_set_expected_nb_tents_row(g, 0, 1);

  result = game_won(g);
  if (result != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);
  return 1;
}

int test_unique_game_fill_grass(game g)
{
  // Récup l'état initial du tableau
  square* tab_init = malloc(g->width * g->height * sizeof(square));
  if (!tab_init) {
    return 0;
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      tab_init[i * g->width + j] = game_get_square(g, i, j);
    }
  }
  game_fill_grass(g);
  // Vérifie que les cases empty ont été remplacé par grass
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (tab_init[i * g->width + j] == EMPTY && game_get_square(g, i, j) != GRASS) {
        fprintf(stderr, "Erreur : une case EMPTY n'a pas été transformée en GRASS");
        free(tab_init);
        return 0;
      }

      else if (tab_init[i * g->width + j] != EMPTY && tab_init[i * g->width + j] != game_get_square(g, i, j)) {
        fprintf(stderr, "Erreur : une case a été modifié alors qu'il ne fallait pas");
        free(tab_init);
        return 0;
      }
    }
  }
  free(tab_init);
  return 1;
}

int test_game_fill_grass()
{
  // A tester : game vide, game normale, game remplie de tente, game remplie
  // d'arbre, game gagnée
  game tab[8];
  tab[0] = game_new_empty();         // game vide
  tab[1] = game_default();           // game normale
  tab[2] = game_new_empty();         // game remplie de tente
  tab[3] = game_new_empty();         // Game remplies d'arbres
  tab[4] = game_default_solution();  // Game gagnée
  tab[5] = create_game_rectangle();
  tab[6] = game_new_empty();
  tab[7] = create_game_rectangle();
  game_set_square(tab[6], 0, 0, BIGTREE);
  game_set_square(tab[6], 0, 1, GRASS);

  for (int i = 0; i < game_nb_rows(tab[2]); i++) {
    for (int j = 0; j < game_nb_cols(tab[2]); j++) {
      game_set_square(tab[2], i, j, TENT);
    }
  }
  for (int i = 0; i < game_nb_rows(tab[3]); i++) {
    for (int j = 0; j < game_nb_cols(tab[3]); j++) {
      game_set_square(tab[3], i, j, TREE);
    }
  }
  game_set_square(tab[7], 1, 1, BIGTREE);
  int result = 1;
  for (int i = 0; i < 8; i++) {
    if (test_unique_game_fill_grass(tab[i]) == 0) {
      result = 0;
    }
    game_delete(tab[i]);
  }
  return result;
}

int test_unique_game_restart(game g)
{
  int tab_init[game_nb_rows(g)][game_nb_cols(g)];
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      tab_init[i][j] = game_get_square(g, i, j);
    }
  }
  game_restart(g);
  // Vérifie que les cases ont été remplacé par empty sauf les arbres
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (tab_init[i][j] != TREE && game_get_square(g, i, j) != EMPTY) {
        fprintf(stderr, "Erreur : une case n'a pas été transformée en empty");
        return 0;
      }

      else if (tab_init[i][j] == TREE && game_get_square(g, i, j) != TREE) {
        fprintf(stderr, "Erreur : Un arbre a été supprimé");
        return 0;
      }
    }
  }
  return 1;
}

int test_game_restart()
{
  // A tester : game vide, game normale, game remplie de tente, game remplie
  // d'arbre, game gagnante
  game tab[6];
  tab[0] = game_new_empty();
  tab[1] = game_default();
  tab[2] = game_new_empty();
  tab[3] = game_new_empty();
  tab[4] = game_default_solution();
  tab[5] = create_game_rectangle();
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      game_set_square(tab[2], i, j, TENT);
      game_set_square(tab[3], i, j, TREE);
    }
  }

  int result = 1;
  for (int i = 0; i < 6; i++) {
    if (test_unique_game_restart(tab[i]) == 0) {
      result = 0;
    }
    game_delete(tab[i]);
  }
  return result;
}

int test_game_is_neigh()
{
  game g_default = game_default();  // valide la condition (ici un arbre dans
                                    // l'entourage de ..)
  game g_solution = game_default_solution();

  // Arbre adjacent (2,2)
  if (game_is_neigh(g_default, 2, 2, TREE, false) != 1) {
    fprintf(stderr,
            "Erreur : Vérifie en (2,1), renvoie false alors qu'il y a un arbre "
            "en (3,1)");
    game_delete(g_default);
    game_delete(g_solution);
    return 0;
  }
  // Arbre en diagonale
  if (game_is_neigh(g_default, 2, 3, TREE, true) != 1) {
    fprintf(stderr,
            "Erreur : Vérifie en (1,1), renvoie false alors qu'il y a un arbre "
            "en diagonale (1,1)");
    game_delete(g_default);
    game_delete(g_solution);
    return 0;
  }
  // si il y a un arbre uniquement en diagonale
  if (game_is_neigh(g_default, 2, 3, TREE, false) != 0) {
    fprintf(stderr,
            "Erreur : Vérifie en (2,3), renvoie true alors qu'il n'y a pas "
            "d'arbre (uniquement en diagonale)");
    game_delete(g_default);
    game_delete(g_solution);
    return 0;
  }
  // pas d'abre autours
  if (game_is_neigh(g_default, 5, 7, TREE, false) != 0) {
    fprintf(stderr,
            "Erreur : Vérifie en (5,7), renvoie true alors qu'il n'y a pas "
            "d'arbre en voisinnage");
    game_delete(g_default);
    game_delete(g_solution);
    return 0;
  }
  // Pas d'abre autours meme en diagonale
  if (game_is_neigh(g_default, 5, 7, TREE, true) != 0) {
    fprintf(stderr,
            "Erreur : Vérifie en (5,7), renvoie true alors qu'il n'y a pas "
            "d'abre en voisinnage (diagonale)");
    game_delete(g_default);
    game_delete(g_solution);
    return 0;
  }
  // en (0,0), sur un arbre mais rien autours
  if (game_is_neigh(g_default, 0, 0, TREE, true) != 0) {
    fprintf(stderr,
            "Erreur : Vérifie en (0,0), renvoie true alors qu'il n'y a pas "
            "d'abre en voisinnage");
    game_delete(g_default);
    game_delete(g_solution);
    return 0;
  }
  if (game_is_neigh(g_solution, 1, 1, TENT, true) != 1) {
    fprintf(stderr, "Erreur : n'a pas trouvé ses voisins tente");
    game_delete(g_default);
    game_delete(g_solution);
    return 0;
  }
  if (game_is_neigh(g_default, 1, 1, TENT, false) != 0) {
    fprintf(stderr, "Erreur : A trouvé un voisin tente alors qu'il ne devrait pas");
    game_delete(g_default);
    game_delete(g_solution);
    return 0;
  }
  // en (0,0), sur un arbre mais rien autours
  if (game_is_neigh(g_solution, 0, 3, TENT, false) != 1) {
    fprintf(stderr, "Erreur : n'a pas trouvé son voisin tente");
    game_delete(g_default);
    game_delete(g_solution);
    return 0;
  }

  game_delete(g_solution);
  game_delete(g_default);
  return 1;
}

int test_game_default()
{
  game g = game_default();
  int arbre_place[12][2] = {{0, 0}, {0, 5}, {0, 6}, {2, 6}, {3, 0}, {3, 2},
                            {4, 4}, {5, 2}, {7, 1}, {7, 2}, {7, 5}, {7, 6}};
  if (game_get_expected_nb_tents_all(g) != 12) {
    return 0;
  }
  // Vérifie si le nb de tente attendue sur chaque colonne est le bon
  if (game_get_expected_nb_tents_col(g, 0) != 3 || game_get_expected_nb_tents_col(g, 1) != 0 ||
      game_get_expected_nb_tents_col(g, 2) != 2 || game_get_expected_nb_tents_col(g, 3) != 1 ||
      game_get_expected_nb_tents_col(g, 4) != 2 || game_get_expected_nb_tents_col(g, 5) != 1 ||
      game_get_expected_nb_tents_col(g, 6) != 2 || game_get_expected_nb_tents_col(g, 7) != 1) {
    return 0;
  }
  if (game_get_expected_nb_tents_row(g, 0) != 1 || game_get_expected_nb_tents_row(g, 1) != 2 ||
      game_get_expected_nb_tents_row(g, 2) != 1 || game_get_expected_nb_tents_row(g, 3) != 2 ||
      game_get_expected_nb_tents_row(g, 4) != 2 || game_get_expected_nb_tents_row(g, 5) != 0 ||
      game_get_expected_nb_tents_row(g, 6) != 1 || game_get_expected_nb_tents_row(g, 7) != 3) {
    return 0;
  }
  if (g == NULL) {
    fprintf(stderr, "Erreur : game_default retourne NULL \n");
    game_delete(g);
    return 0;
  }
  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      int t = 0;
      for (int m = 0; m < 12; m++) {
        if (arbre_place[m][0] == i && arbre_place[m][1] == j) {
          t = 1;
        }
      }
      if (t == 1 && game_get_square(g, i, j) != TREE) {
        game_delete(g);
        return 0;
      }
      if (t == 0 && game_get_square(g, i, j) != EMPTY) {
        game_delete(g);
        return 0;
      }
    }
  }

  game_delete(g);
  return 1;
}

int test_game_default_solution()
{
  game g = game_default_solution();
  game_print((cgame)g);
  if (g == NULL) {
    fprintf(stderr, "Erreur : game_default retourne NULL \n");
    return 0;
  }
  int trees[][2] = {{0, 0}, {0, 5}, {0, 6}, {2, 6}, {3, 0}, {3, 2}, {4, 4}, {5, 2}, {7, 1}, {7, 2}, {7, 5}, {7, 6}};
  int size_trees = 12;
  int size_tents = 12;
  int tent[][2] = {{0, 4}, {1, 0}, {1, 6}, {2, 2}, {3, 4}, {3, 6}, {4, 0}, {4, 2}, {6, 5}, {7, 0}, {7, 3}, {7, 7}};
  int grass[][2] = {{0, 1}, {0, 2}, {0, 3}, {0, 7}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 7},
                    {2, 0}, {2, 1}, {2, 3}, {2, 4}, {2, 5}, {2, 7}, {3, 1}, {3, 3}, {3, 5}, {3, 7},
                    {4, 1}, {4, 3}, {4, 5}, {4, 6}, {4, 7}, {5, 0}, {5, 1}, {5, 3}, {5, 4}, {5, 5},
                    {5, 6}, {5, 7}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 6}, {6, 7}, {7, 4}};
  int size_grass = 40;
  for (int i = 0; i < size_trees; i++) {
    if (game_get_square(g, trees[i][0], trees[i][1]) != TREE) {
      game_delete(g);
      return 0;
    }
  }
  for (int i = 0; i < size_tents; i++) {
    if (game_get_square(g, tent[i][0], tent[i][1]) != TENT) {
      game_delete(g);
      return 0;
    }
  }
  for (int i = 0; i < size_grass; i++) {
    if (game_get_square(g, grass[i][0], grass[i][1]) != GRASS) {
      game_delete(g);
      return 0;
    }
  }
  game_delete(g);
  return 1;
}

int test_game_print()
{
  game g = game_default();
  game_print((cgame)g);
  game_delete(g);
  return 1;
}

int test_game_get_adjacent_square()
{
  game g = game_default();
  game_print((cgame)g);
  uint pi_next;
  uint pj_next;
  if (game_get_adjacent_square(g, 0, 0, UP_LEFT, &pi_next, &pj_next) != 0) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, game_nb_rows(g) - 1, game_nb_cols(g) - 1, DOWN_RIGHT, &pi_next, &pj_next) != 0) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 1, 1, UP_LEFT, &pi_next, &pj_next) != 1 || pi_next != 0 || pj_next != 0) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 3, 3, DOWN, &pi_next, &pj_next) != 1 || pi_next != 4 || pj_next != 3) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 3, 3, DOWN_RIGHT, &pi_next, &pj_next) != 1 || pi_next != 4 || pj_next != 4) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 3, 3, RIGHT, &pi_next, &pj_next) != 1 || pi_next != 3 || pj_next != 4) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 3, 3, UP_RIGHT, &pi_next, &pj_next) != 1 || pi_next != 2 || pj_next != 4) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 3, 3, UP, &pi_next, &pj_next) != 1 || pi_next != 2 || pj_next != 3) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 3, 3, UP_LEFT, &pi_next, &pj_next) != 1 || pi_next != 2 || pj_next != 2) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 3, 3, LEFT, &pi_next, &pj_next) != 1 || pi_next != 3 || pj_next != 2) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 3, 3, DOWN_LEFT, &pi_next, &pj_next) != 1 || pi_next != 4 || pj_next != 2) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 2, 2, HERE, &pi_next, &pj_next) != 1 || pi_next != 2 || pj_next != 2) {
    game_delete(g);
    return 0;
  }
  if (game_get_adjacent_square(g, 0, 3, UP, &pi_next, &pj_next) != 0) {
    game_delete(g);
    return 0;
  }

  game_delete(g);
  return 1;
}

int test_game_nb_neigh()
{
  game g = game_new_empty_ext(8, 8, false);

  game_set_square(g, 1, 1, TREE);
  game_set_square(g, 0, 1, TREE);
  game_set_square(g, 2, 2, TREE);

  if (game_nb_neigh(g, 1, 1, TREE, false) != 1) {
    game_delete(g);
    return 0;
  }

  if (game_nb_neigh(g, 1, 1, TREE, true) != 2) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  game g_wrap = game_new_empty_ext(8, 8, true);
  game_set_square(g_wrap, 0, 0, TREE);
  if (game_nb_neigh(g_wrap, 0, 7, TREE, false) != 1) {
    game_delete(g_wrap);
    return 0;
  }
  game_delete(g_wrap);
  return 1;
}

int test_game_new_empty_ext(void)
{
  game g = game_new_empty_ext(10, 5, false);
  if (g == NULL) {
    game_delete(g);
    return 0;
  }
  if (game_nb_rows(g) != 10) {
    game_delete(g);
    return 0;
  }
  if (game_nb_cols(g) != 5) {
    game_delete(g);
    return 0;
  }
  if (game_is_wrapping(g) != false) {
    game_delete(g);
    return 0;
  }

  for (int i = 0; i < game_nb_rows(g); i++) {
    for (int j = 0; j < game_nb_cols(g); j++) {
      if (game_get_square(g, i, j) != EMPTY) {
        game_delete(g);
        return 0;
      }
    }
    if (game_get_expected_nb_tents_row(g, i) != 0) {
      game_delete(g);
      return 0;
    }
  }
  for (int i = 0; i < game_nb_cols(g); i++) {
    if (game_get_expected_nb_tents_col(g, i) != 0) {
      game_delete(g);
      return 0;
    }
  }
  if (game_get_current_nb_tents_all(g) != 0) {
    game_delete(g);
    return 0;
  }
  game_delete(g);

  game g2 = game_new_empty_ext(4, 8, true);
  if (g2 == NULL) {
    game_delete(g2);
    return 0;
  }
  if (game_nb_rows(g2) != 4) {
    game_delete(g2);
    return 0;
  }
  if (game_nb_cols(g2) != 8) {
    game_delete(g2);
    return 0;
  }
  if (game_is_wrapping(g2) != true) {
    game_delete(g2);
    return 0;
  }

  game_delete(g2);
  game g3 = game_new_empty_ext(5, 5, true);
  if (g3 == NULL) {
    return 0;
  }
  if (game_nb_rows(g3) != 5) {
    game_delete(g3);
    return 0;
  }
  if (game_nb_cols(g3) != 5) {
    game_delete(g3);
    return 0;
  }
  if (game_is_wrapping(g3) != true) {
    game_delete(g3);
    return 0;
  }

  game_delete(g3);
  return 1;
}

int test_game_get_expected_nb_tents_all()
{
  // TESTS SUR : un empty, en additionnant le nombre voulu pour chaque ligne et
  // chaque colonne

  game g_empty = game_new_empty();
  game g = game_default();
  if (game_get_expected_nb_tents_all(g_empty) != 0) {
    game_delete(g_empty);
    return 0;
  }

  int a = 0;
  for (int i = 0; i < game_nb_cols(g); i++) {
    a = a + game_get_expected_nb_tents_col(g, i);
  }

  if (a != game_get_expected_nb_tents_all(g)) {
    game_delete(g);
    return 0;
  }

  game_delete(g_empty);
  game_delete(g);
  return 1;
}

int test_dummy() { return 1; }

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
  if (strcmp("game_won", argv[1]) == 0) {
    ok = test_game_won();
  }
  if (strcmp("game_fill_grass", argv[1]) == 0) {
    ok = test_game_fill_grass();
  }
  if (strcmp("game_restart", argv[1]) == 0) {
    ok = test_game_restart();
  }
  if (strcmp("game_is_neigh", argv[1]) == 0) {
    ok = test_game_is_neigh();
  }
  if (strcmp("game_default", argv[1]) == 0) {
    ok = test_game_default();
  }
  if (strcmp("game_default_solution", argv[1]) == 0) {
    ok = test_game_default_solution();
  }
  if (strcmp("game_print", argv[1]) == 0) {
    ok = test_game_print();
  }
  if (strcmp("game_get_adjacent_square", argv[1]) == 0) {
    ok = test_game_get_adjacent_square();
  }
  if (strcmp("game_get_expected_nb_tents_all", argv[1]) == 0) {
    ok = test_game_get_expected_nb_tents_all();
  }
  if (strcmp("game_new_empty_ext", argv[1]) == 0) {
    ok = test_game_new_empty_ext();
  }
  if (strcmp("game_nb_neigh", argv[1]) == 0) {
    ok = test_game_nb_neigh();
  }
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}