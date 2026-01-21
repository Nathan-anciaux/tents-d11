#include "game_aux.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_ext.h"
#include "game_struct.h"

void game_print(cgame g) {  // affiche la grille et les infos correspondants (nb ligne nb
                            // col et les tentes attendues sur le cotes et en bas)
  printf("  ");
  for (int i = 0; i < g->width; i++) {
    printf("%d", i);
  }
  printf("\n  ");
  for (int i = 0; i < g->width; i++) {
    printf("-");
  }
  printf("\n");
  for (int i = 0; i < g->height; i++) {
    printf("%d|", i);
    for (int j = 0; j < g->width; j++) {
      int valeur_case = game_get_square(g, i, j);
      if (valeur_case == TREE) {
        printf("x");
      } else if (valeur_case == TENT) {
        printf("*");

      } else if (valeur_case == GRASS) {
        printf("-");

      } else if (valeur_case == BIGTREE) {
        printf("#");
      } else {
        printf(" ");
      }
    }
    printf("|%d\n", g->expected_row[i]);
  }
  for (int i = 0; i < g->width / 2; i++) {
    printf("-");
  }
  for (int w = 0; w < g->width; w++) {
    printf("%d", g->expected_col[w]);
  }
  for (uint w = 0; w < g->width / 2; w++) printf("-");
  printf("\n");
}

game game_default(void) {
  game g = game_new_empty();
  for (uint i = 0; i < g->height; i++) {
    for (uint j = 0; j < g->width; j++) {
      if (i == 0 && j == 0) {
        g->grille[i][j] = TREE;
      } else if (i == 0 && j == 5) {
        g->grille[i][j] = TREE;
      } else if (i == 0 && j == 6) {
        g->grille[i][j] = TREE;
      } else if (i == 2 && j == 6) {
        g->grille[i][j] = TREE;
      } else if (i == 3 && j == 0) {
        g->grille[i][j] = TREE;
      } else if (i == 3 && j == 2) {
        g->grille[i][j] = TREE;
      } else if (i == 4 && j == 4) {
        g->grille[i][j] = TREE;
      } else if (i == 5 && j == 2) {
        g->grille[i][j] = TREE;
      } else if (i == 7 && j == 1) {
        g->grille[i][j] = TREE;
      } else if (i == 7 && j == 2) {
        g->grille[i][j] = TREE;
      } else if (i == 7 && j == 5) {
        g->grille[i][j] = TREE;
      } else if (i == 7 && j == 6) {
        g->grille[i][j] = TREE;
      } else {
        g->grille[i][j] = EMPTY;
      }
    }
  }
  uint expected_col[DEFAULT_SIZE] = {3, 0, 2, 1, 2, 1, 2, 1};
  uint expected_row[DEFAULT_SIZE] = {1, 2, 1, 2, 2, 0, 1, 3};
  for (uint i = 0; i < g->height; i++) {
    g->expected_row[i] = expected_row[i];
  }
  for (uint i = 0; i < g->width; i++) {
    g->expected_col[i] = expected_col[i];
  }

  return g;
}

game game_default_solution(void) {  // copie la solution du jeu initial de maniere
  game g = game_default();
  g->grille[0][4] = TENT;
  g->grille[1][0] = TENT;
  g->grille[1][6] = TENT;
  g->grille[2][2] = TENT;
  g->grille[3][4] = TENT;
  g->grille[3][6] = TENT;
  g->grille[4][0] = TENT;
  g->grille[4][2] = TENT;
  g->grille[6][5] = TENT;
  g->grille[7][0] = TENT;
  g->grille[7][3] = TENT;
  g->grille[7][7] = TENT;
  game_fill_grass(g);
  return g;
}

bool game_get_adjacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next) {
  assert(g != NULL);
  assert(i < g->height);
  assert(j < g->width);

  int pi = i;
  int pj = j;

  // Déplacement normal (sans wrap)
  if (d == UP || d == UP_LEFT || d == UP_RIGHT) {
    pi -= 1;
  }
  if (d == DOWN || d == DOWN_LEFT || d == DOWN_RIGHT) {
    pi += 1;
  }
  if (d == UP_LEFT || d == LEFT || d == DOWN_LEFT) {
    pj -= 1;
  }
  if (d == UP_RIGHT || d == RIGHT || d == DOWN_RIGHT) {
    pj += 1;
  }

  // Cas hors grille, on vérifie si la game est wrap sinon on retourne false
  if (pi < 0) {
    if (!game_is_wrapping(g)) return false;
    pi = g->height - 1;
  }
  if (pj < 0) {
    if (!game_is_wrapping(g)) return false;
    pj = g->width - 1;
  }
  if (pi >= g->height) {
    if (!game_is_wrapping(g)) return false;
    pi = 0;
  }
  if (pj >= g->width) {
    if (!game_is_wrapping(g)) return false;
    pj = 0;
  }

  // On met à jour les valeurs des pointeurs
  *pj_next = pj;
  *pi_next = pi;
  return true;
}

uint game_nb_neigh(cgame g, uint i, uint j, square s, bool diag) {
  uint count = 0;
  uint pi;
  uint pj;

  // On verifie les cases orthogonales
  if (game_get_adjacent_square(g, i, j, UP, &pi, &pj) && game_get_square(g, pi, pj) == s) {
    count++;
  }
  if (game_get_adjacent_square(g, i, j, DOWN, &pi, &pj) && game_get_square(g, pi, pj) == s) {
    count++;
  }
  if (game_get_adjacent_square(g, i, j, RIGHT, &pi, &pj) && game_get_square(g, pi, pj) == s) {
    count++;
  }
  if (game_get_adjacent_square(g, i, j, LEFT, &pi, &pj) && game_get_square(g, pi, pj) == s) {
    count++;
  }

  // On verifie cette fois ci les cases diagonales
  if (diag == true) {
    if (game_get_adjacent_square(g, i, j, UP_LEFT, &pi, &pj) && game_get_square(g, pi, pj) == s) {
      count++;
    }
    if (game_get_adjacent_square(g, i, j, UP_RIGHT, &pi, &pj) && game_get_square(g, pi, pj) == s) {
      count++;
    }
    if (game_get_adjacent_square(g, i, j, DOWN_LEFT, &pi, &pj) && game_get_square(g, pi, pj) == s) {
      count++;
    }
    if (game_get_adjacent_square(g, i, j, DOWN_RIGHT, &pi, &pj) && game_get_square(g, pi, pj) == s) {
      count++;
    }
  }

  return count;
}

bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag) {
  assert(g != NULL);

  // On verifie les cases orthogonales et diagonales
  if (game_nb_neigh(g, i, j, s, diag) >= 1) return true;

  return false;
}
