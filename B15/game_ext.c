#include "game_ext.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_struct.h"
#include "queue.h"

game game_new_ext(uint nb_rows, uint nb_cols, square* squares, uint* nb_tents_row, uint* nb_tents_col, bool wrapping) {
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);
  // On copie les cases de la grille par défaut
  for (uint i = 0; i < g->height; i++) {
    for (uint j = 0; j < g->width; j++) {
      g->grille[i][j] = squares[i * g->width + j];
    }
  }
  for (uint i = 0; i < g->width; i++) {
    g->expected_col[i] = nb_tents_col[i];
  }
  for (uint j = 0; j < g->height; j++) {
    g->expected_row[j] = nb_tents_row[j];
  }

  return g;
}

game game_new_empty_ext(uint nb_rows, uint nb_cols, bool wrapping) {
  game g = malloc(sizeof(struct game_s));
  if (g == NULL) {
    fprintf(stderr, "Erreur d'allocation mémoire de la game\n");
    exit(EXIT_FAILURE);
  }

  g->height = nb_rows;
  g->width = nb_cols;
  g->is_wrap = wrapping;
  g->undo = queue_new();
  g->redo = queue_new();

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

uint game_nb_rows(cgame g) {
  assert(g != NULL);
  return g->height;
}

uint game_nb_cols(cgame g) {
  assert(g != NULL);
  return g->width;
}

bool game_is_wrapping(cgame g) {
  assert(g != NULL);
  printf("%u", g->is_wrap);
  return g->is_wrap;
}

void game_undo(game g) {
  assert(g != NULL);
  if (queue_is_empty(g->undo)) return;
  game snapshot_undo = queue_peek_head(g->undo);  // récupere la dérniere version du plateau antérieur
  game snapshot_redo = game_copy(g);              // on ajoute l'état actuel du jeux dans le redo

  for (uint i = 0; i < g->height; i++) {
    for (uint j = 0; j < g->width; j++) {
      g->grille[i][j] = snapshot_undo->grille[i][j];  // copie de la grille
    }
  }
  for (uint i = 0; i < g->width; i++) {
    g->expected_col[i] = snapshot_undo->expected_col[i];
  }
  for (uint j = 0; j < g->height; j++) {
    g->expected_row[j] = snapshot_undo->expected_row[j];
  }
  g->is_wrap = snapshot_undo->is_wrap;
  queue_pop_head(g->undo);
  queue_push_head(g->redo, snapshot_redo);
  game_delete(snapshot_undo);
  return;
}

void game_redo(game g) {
  assert(g != NULL);
  if (queue_is_empty(g->redo)) return;
  game snapshot_undo = game_copy(g);  // on met l'état actuel du jeux dans le undo
  queue_push_head(g->undo, snapshot_undo);
  game snapshot_redo = queue_peek_head(g->redo);
  for (uint i = 0; i < g->height; i++) {
    for (uint j = 0; j < g->width; j++) {
      g->grille[i][j] = snapshot_redo->grille[i][j];
    }
  }
  for (uint i = 0; i < g->width; i++) {
    g->expected_col[i] = snapshot_redo->expected_col[i];
  }
  for (uint i = 0; i < g->height; i++) {
    g->expected_row[i] = snapshot_redo->expected_row[i];
  }
  queue_pop_head(g->redo);
  g->is_wrap = snapshot_redo->is_wrap;
  game_delete(snapshot_redo);
  return;
}
