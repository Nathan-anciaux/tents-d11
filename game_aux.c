// game_aux.c
#include "game_aux.h"

#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_struct.h"

static bool on_board(cgame g, int i, int j) { return i >= 0 && i < g->height && j >= 0 && j < g->width; }

typedef struct {
  int row;
  int col;
} coord;

static bool get_direction(direction d, int* di, int* dj)
{
  static const coord coordonnees[] = {{0, 0}, {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  *di = coordonnees[d].row;
  *dj = coordonnees[d].col;
  return 1;
}

void print_horizontal_border(int height)
// Auxiliary function to print an horizontal border
{
  printf("   ");
  for (int i = 0; i < height; i++) {
    printf("_ ");
  }
  printf("\n");
}

void print_square(square s)
// Auxiliary function to print a square
{
  switch (s) {
    case EMPTY:
      printf("  ");
      break;
    case TREE:
      printf("X ");
      break;
    case TENT:
      printf("* ");
      break;
    case GRASS:
      printf("- ");
      break;
    case BIGTREE:
      printf("# ");
      break;
  }
}

void game_print(cgame g)
// Prints a game as text on the standard output stream.
{
  // Print of each column index
  printf("   ");
  for (int i = 0; i < g->width; i++) {
    printf("%d ", i);
  }
  printf("\n");
  // Upper border
  print_horizontal_border(g->height);
  // Print in the following order : line index, border, game, border, nb of
  // expected tent
  for (int i = 0; i < g->height; i++) {
    printf("%d |", i);
    for (int j = 0; j < g->width; j++) {
      print_square(game_get_square(g, i, j));
    }

    printf("| %d", g->nb_tent_row_expected[i]);
    printf("\n");
  }
  // lower border
  print_horizontal_border(g->height);
  // Print of nb expected tents column
  printf("   ");
  for (int i = 0; i < g->width; i++) {
    printf("%d ", g->nb_tent_col_expected[i]);
  }
  printf("\n");

  return;
}

game game_default(void)
// Creates the default game
{
  uint expected_tent_row[DEFAULT_SIZE] = {1, 2, 1, 2, 2, 0, 1, 3};
  uint expected_tent_col[DEFAULT_SIZE] = {3, 0, 2, 1, 2, 1, 2, 1};
  square tab[] = {TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, TREE,  EMPTY,
                  TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY,
                  EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
                  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE,  EMPTY};

  game g = game_new(tab, expected_tent_row, expected_tent_col);

  return g;
}

game game_default_solution(void)
// Creates the default game solution.
{
  square tab[DEFAULT_SIZE * DEFAULT_SIZE] = {
      TREE,  GRASS, GRASS, GRASS, TENT,  TREE,  TREE,  GRASS, TENT,  GRASS, GRASS, GRASS, GRASS, GRASS, TENT,  GRASS,
      GRASS, GRASS, TENT,  GRASS, GRASS, GRASS, TREE,  GRASS, TREE,  GRASS, TREE,  GRASS, TENT,  GRASS, TENT,  GRASS,
      TENT,  GRASS, TENT,  GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS, TREE,  GRASS, GRASS, GRASS, GRASS, GRASS,
      GRASS, GRASS, GRASS, GRASS, GRASS, TENT,  GRASS, GRASS, TENT,  TREE,  TREE,  TENT,  GRASS, TREE,  TREE,  TENT};

  uint expected_tent_row[DEFAULT_SIZE] = {1, 2, 1, 2, 2, 0, 1, 3};
  uint expected_tent_col[DEFAULT_SIZE] = {3, 0, 2, 1, 2, 1, 2, 1};

  game g = game_new(tab, expected_tent_row, expected_tent_col);

  return g;
}

bool game_get_adjacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next)
// Gets the coordinate of the adjacent square in a given direction.
{
  int row, col;
  get_direction(d, &row, &col);
  int new_row = i + row;
  int new_col = j + col;
  if (!g->wrapped)
    if (!on_board(g, new_row, new_col)) return 0;

  // POUR WRAPPED
  //  regarde la direction qu'on a et suivant celle ci si mon nbr de colonne /
  //  ligne excede on va modulo ou revenir au debut quoi
  // franchement si c'est ça c'est vraiment facile i = -1 modulo 7 = 6 --> la
  // derniere ligne et la même pour les colonne
  *pi_next = new_row % g->height;
  *pj_next = new_col % g->width;

  return 1;
}

uint game_nb_neigh(cgame g, uint i, uint j, square s, bool diag)
{
  if (!game_is_neigh(g, i, j, s, diag))
    return 0;
  else {
    int val = 8;
    uint nb_neigh = 0;
    int test[] = {-1, 0, 1, 0, 0, -1, 0, 1, -1, -1, -1, 1, 1, -1, 1, 1};
    if (diag) val = 16;
    for (int k = 0; k < val; k = k + 2) {
      if (!g->wrapped) {
        // on fait comme precedemment
        if ((int)i + test[k] >= 0 && (int)i + test[k] < g->height && (int)j + test[k + 1] >= 0 &&
            (int)j + test[k + 1] < g->width)
          if (game_get_square(g, (int)i + test[k], (int)j + test[k + 1]) == s) nb_neigh++;
      } else {
        // on regarde avec les modulos pour que ce soit des cases dans le
        // plateau similaire a adjacent square
        if (game_get_square(g, (((int)i + test[k]) % g->height + g->height) % g->height,
                            (((int)j + test[k + 1]) % g->width + g->width) % g->width) == s)
          nb_neigh++;
      }
    }
    return nb_neigh;
  }
}

bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag)
// Determines if a neighboring square has a given value
{
  int val = 8;
  int test[] = {-1, 0, 1, 0, 0, -1, 0, 1, -1, -1, -1, 1, 1, -1, 1, 1};
  if (diag) val = 16;
  for (int k = 0; k < val; k = k + 2) {
    if (!g->wrapped) {
      // on fait comme precedemment
      if (i + test[k] >= 0 && i + test[k] < DEFAULT_SIZE && j + test[k + 1] >= 0 && j + test[k + 1] < DEFAULT_SIZE)
        if (game_get_square(g, i + test[k], j + test[k + 1]) == s) return 1;
    } else {
      // on regarde avec les modulos pour que ce soit des cases dans le plateau
      // similaire a adjacent square
      if (game_get_square(g, (i + test[k]) % g->height, (j + test[k + 1]) % g->width) == s) return 1;
    }
  }
  return 0;
}
