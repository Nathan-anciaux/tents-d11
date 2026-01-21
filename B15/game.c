#include "game.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

game game_new(square* grille, uint* nb_tents_row, uint* nb_tents_col) {
  game g = game_new_empty();
  // On copie les cases de la grille par défaut
  for (uint i = 0; i < g->height; i++) {
    for (uint j = 0; j < g->width; j++) {
      g->grille[i][j] = grille[i * g->width + j];
    }
  }
  for (uint i = 0; i < g->height; i++) {
    g->expected_row[i] = nb_tents_row[i];
  }
  for (uint j = 0; j < g->width; j++) {
    g->expected_col[j] = nb_tents_col[j];
  }

  return g;
}

game game_new_empty(void) {
  game g = malloc(sizeof(struct game_s));
  if (g == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire de la game\n");
    exit(EXIT_FAILURE);
  }
  g->undo = queue_new();
  g->redo = queue_new();
  g->height = DEFAULT_SIZE;
  g->width = DEFAULT_SIZE;
  g->is_wrap = false;

  // Allocation de toutes les cases a EMPTY
  g->grille = malloc(g->height * sizeof(square*));
  if (g->grille == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire de la grille)\n");
    free(g);
    exit(EXIT_FAILURE);
  }

  for (uint i = 0; i < g->height; i++) {
    g->grille[i] = malloc(g->width * sizeof(square));
    if (g->grille[i] == NULL) {
      fprintf(stderr, "Erreur d'allocation mémoire ligne %u\n", i);
      for (uint k = 0; k < i; k++) {
        free(g->grille[k]);
      }
      free(g->grille);
      free(g);
      exit(EXIT_FAILURE);
    }

    for (uint j = 0; j < g->width; j++) {
      g->grille[i][j] = EMPTY;
    }
  }

  // Allocation simple car tout est a 0
  g->expected_col = calloc(g->width, sizeof(uint));
  g->expected_row = calloc(g->height, sizeof(uint));
  if (g->expected_row == NULL || g->expected_col == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire sur les expected\n");
    free(g->grille);
    free(g);
    exit(EXIT_FAILURE);
  }

  return g;
}

game game_copy(cgame g) {
  if (g == NULL) {
    fprintf(stderr, "Erreur : pointeur de jeu NULL dans game_copy\n");
    return NULL;
  }

  game h = game_new_empty_ext(g->height, g->width, g->is_wrap);
  h->height = g->height;
  h->width = g->width;
  h->is_wrap = g->is_wrap;

  for (uint i = 0; i < h->height; i++) {
    for (uint j = 0; j < h->width; j++) {
      h->grille[i][j] = g->grille[i][j];
    }
  }

  // On copie maintenant les expected row et col
  for (uint i = 0; i < h->width; i++) {
    h->expected_col[i] = g->expected_col[i];
  }
  for (uint i = 0; i < h->height; i++) {
    h->expected_row[i] = g->expected_row[i];
  }

  return h;
}

bool game_equal(cgame g1, cgame g2) {
  if (g1 == NULL || g2 == NULL) {
    return false;
  }
  // On vérifie la taille
  if ((g1->width != g2->width) || g1->height != g2->height) {
    return false;
  }
  // On vérifie chaque case du tableau
  for (uint i = 0; i < g1->height; i++) {
    for (uint j = 0; j < g1->width; j++) {
      if (g1->grille[i][j] != g2->grille[i][j]) {
        return false;
      }
    }
  }
  // On vérifie les expected row et col
  for (uint i = 0; i < g1->width; i++) {
    if (g1->expected_col[i] != g2->expected_col[i]) return false;
  }
  for (uint i = 0; i < g1->height; i++) {
    if (g1->expected_row[i] != g2->expected_row[i]) return false;
  }

  if (g1->is_wrap != g2->is_wrap) return false;

  return true;
}

void game_delete(game g) {
  if (g == NULL) {
    return;
  }
  if (g->grille != NULL) {
    for (uint i = 0; i < g->height; i++) {
      free(g->grille[i]);
    }
    free(g->grille);
  }
  free(g->expected_col);
  free(g->expected_row);
  // queue_free(g->undo);
  // queue_free(g->redo);
  if (g->undo) {
    queue_free_full(g->undo, (void (*)(void*))game_delete);
  }
  if (g->redo) {
    queue_free_full(g->redo, (void (*)(void*))game_delete);
  }
  free(g);
  return;
}

// Place un carré s à la position (i,j) de la grille du jeu g
void game_set_square(game g, uint i, uint j, square s) {
  assert(g != NULL || i < g->height || j < g->width || (s == EMPTY || s == GRASS || s == TENT));
  g->grille[i][j] = s;
}

// Renvoie le carré à la position (i,j) de la grille du jeu g
square game_get_square(cgame g, uint i, uint j) {
  assert(g != NULL || i < g->height || j < g->width);
  return g->grille[i][j];
}

void game_set_expected_nb_tents_row(game g, uint i,
                                    uint nb_tents) {  // définie le nombre de TENTS attendue pour la ligne i
                                                      // dans le tableau expected_row
  g->expected_row[i] = nb_tents;
}

void game_set_expected_nb_tents_col(game g, uint j,
                                    uint nb_tents) {  // définie le nombre de TENTS attendue pour la ligne j
                                                      // dans le tableau expected_col
  g->expected_col[j] = nb_tents;
}

uint game_get_expected_nb_tents_row(cgame g, uint i) {  // renvoie le nombre de TENTS attendue pour la ligne i
                                                        // grace au tableau expected_row
  return g->expected_row[i];
}

uint game_get_expected_nb_tents_col(cgame g, uint j) {  // renvoie le nombre de TENTS attendue pour la ligne j
                                                        // grace au tableau expected_col
  return g->expected_col[j];
}

uint game_get_expected_nb_tents_all(cgame g) {  // renvoie le nombre de TENTS attendue pour le jeu entier grace
                                                // au tableau expected_row
  if (g == NULL) return 0;
  uint all_row = 0, all_col = 0;

  for (uint i = 0; i < g->height; i++) {
    all_row += g->expected_row[i];
  }
  for (uint j = 0; j < g->width; j++) {
    all_col += g->expected_col[j];
  }
  if (all_row > 0) {
    return all_row;
  }
  if (all_col > 0) {
    return all_col;
  }
  return 0;
}

// Retourne le nombre de tentes placées dans la ligne i
uint game_get_current_nb_tents_row(cgame g, uint i) {
  assert(g != NULL && i < g->height);
  uint nb_tents = 0;
  for (int j = 0; j < g->width; j++) {
    if (g->grille[i][j] == TENT) {
      nb_tents += 1;
    }
  }
  return nb_tents;
}

// Retourne le nombre de tentes placées dans la colonne j
uint game_get_current_nb_tents_col(cgame g, uint j) {
  assert(g != NULL && j < g->width);
  uint nb_tents = 0;
  for (int i = 0; i < g->height; i++) {
    if (g->grille[i][j] == TENT) {
      nb_tents += 1;
    }
  }
  return nb_tents;
}

// Retourne le nombre de tentes placées dans le jeu entier
uint game_get_current_nb_tents_all(cgame g) {
  assert(g != NULL);
  uint nb_tents = 0;
  for (int i = 0; i < g->height; i++) {
    nb_tents += game_get_current_nb_tents_row(g, i);
  }
  return nb_tents;
}

// Joue un coup en plaçant le carré s à la position (i,j) de la grille du jeu g
void game_play_move(game g, uint i, uint j, square s) {
  assert(g != NULL);
  assert(i < g->height && j < g->width);
  assert(s == EMPTY || s == GRASS || s == TENT);
  assert(game_get_square(g, i, j) != TREE && game_get_square(g, i, j) != BIGTREE);

  // Sauvegarde de l'état actuel du jeu avant de jouer le coup (UNDO)
  game save = game_copy(g);
  queue_push_head(g->undo, save);

  g->grille[i][j] = s;
  return;
}

// Vérifie si le coup s en (i,j) est valide
bool game_check_move(cgame g, uint i, uint j, square s) {
  assert(g != NULL);
  assert(s == EMPTY || s == GRASS || s == TENT || s == TREE || s == BIGTREE);

  if (i >= g->height || j >= g->width) {  // Cas hors grille
    return false;
  }
  if (s == TREE || s == BIGTREE) {
    return false;
  }
  if (g->grille[i][j] == TREE || g->grille[i][j] == BIGTREE) {
    return false;
  }
  return true;
}

// Vérifie si le jeu est gagné
bool game_won(cgame g) {
  assert(g != NULL);

  // Regle 2: Vérifie si le nombre de tentes par ligne est correct
  for (int i = 0; i < g->height; i++) {
    if (game_get_current_nb_tents_row(g, i) != game_get_expected_nb_tents_row(g, i)) return false;
  }
  // Regle 2: Vérifie si le nombre de tentes par colonne est correct
  for (int j = 0; j < g->width; j++) {
    if (game_get_current_nb_tents_col(g, j) != game_get_expected_nb_tents_col(g, j)) return false;
  }

  uint nb_tree = 0;
  for (int i = 0; i < g->height; i++) {
    for (int j = 0; j < g->width; j++) {
      if (g->grille[i][j] == TENT) {
        // Regle 1: deux tentes sont adjacentes
        if (game_is_neigh(g, i, j, TENT, true)) return false;
        // Regle 4: Une tente n'est pas adjacente a un arbre ou bigtree
        if (!game_is_neigh(g, i, j, TREE, false) && !game_is_neigh(g, i, j, BIGTREE, false)) return false;
      }

      if (g->grille[i][j] == TREE) {
        // Compte le nombre d'arbre sur la grille
        if (game_nb_neigh(g, i, j, TENT, false) < 1) return false;
        nb_tree += 1;
      }

      if (g->grille[i][j] == BIGTREE) {
        // Un bigtree doit avoir exactement 2 tentes adjacentes
        if (game_nb_neigh(g, i, j, TENT, false) != 2) return false;
        nb_tree += 2;
      }
    }
  }
  // Regle 3 nb tentes = nb arbres
  if (nb_tree != game_get_current_nb_tents_all(g)) return false;
  return true;
}

void game_fill_grass(game g) {
  if (g == NULL) {
    fprintf(stderr, "Erreur : pointeur NULL game_fill_grass\n");
    return;
  }

  game save = game_copy(g);
  assert(save != NULL);
  queue_push_head(g->undo, save);  // on sauvegarde l'état du jeux pour un potentiel retour

  for (uint i = 0; i < g->height; i++) {
    for (uint j = 0; j < g->width; j++) {
      if (g->grille[i][j] == EMPTY) {
        g->grille[i][j] = GRASS;
      }
    }
  }
}

void game_restart(game g) {  // vide le jeu à l'exception des arbres en
                             // parcourant toute la grille
  for (uint i = 0; i < g->height; i++) {
    for (uint j = 0; j < g->width; j++) {
      if (g->grille[i][j] != TREE && g->grille[i][j] != BIGTREE) {
        g->grille[i][j] = EMPTY;
      }
    }
  }
  while (!queue_is_empty(g->undo)) {
    game tmp = queue_peek_head(g->undo);
    game_delete(tmp);
    queue_pop_head(g->undo);
  }

  while (!queue_is_empty(g->redo)) {
    game tmp = queue_peek_head(g->redo);
    game_delete(tmp);
    queue_pop_head(g->redo);
  }
}
