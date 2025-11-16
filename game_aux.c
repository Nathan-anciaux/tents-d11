// game_aux.c
#include <stdio.h>

#include "game_struct.h"

void game_print(cgame g)
// Prints a game as text on the standard output stream.
{
  // Affichage d'abord des indices de chaque colonne
  printf("   ");
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    printf("%d ", i);
  }
  printf("\n");
  // Bordure supérieure
  printf("   ");
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    printf("_ ");
  }
  printf("\n");
  // Affichage dans l'ordre de : l'indice de la ligne, les bordures, le jeu, une bordure, le nombre de tentes attendues
  // sur la ligne
  for (int i = 0; i < DEFAULT_SIZE; i++) {
    printf("%d |", i);
    for (int j = 0; j < DEFAULT_SIZE; j++) {
      if (game_get_square(g, i, j) == EMPTY) {
        printf("  ");
      }
      if (game_get_square(g, i, j) == TREE) {
        printf("X ");
      }
      if (game_get_square(g, i, j) == TENT) {
        printf("* ");
      }
      if (game_get_square(g, i, j) == GRASS) {
        printf("- ");
      }
    }
    
    printf("| %d", g->nb_tent_row_expected[i]);
    printf("\n");
  }
  //Affichage bordure inférieure
  printf("   ");
  for (int i=0; i<DEFAULT_SIZE; i++){
    printf("_ ");
  }
  printf("\n");
  //Affichage nb tente attendues colonnes
  printf("   ");
  for (int i=0; i<DEFAULT_SIZE; i++){
    printf("%d ", g->nb_tent_col_expected[i]);
  }
  printf("\n");

  return;
}

game game_default(void)
// Creates the default game
{
  game g = NULL;
  return g;
}

game game_default_solution(void)
// Creates the default game solution.
{
  game g = NULL;
  return g;
}

bool game_get_ajacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next)
// Gets the coordinate of the adjacent square in a given direction.
{
  return 1;
}

bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag)
// Determines if a neighboring square has a given value
{
  return 1;
}