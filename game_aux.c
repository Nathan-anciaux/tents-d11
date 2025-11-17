// game_aux.c
#include <stdio.h>

#include "game_struct.h"
#include "game.h"
#include "game_aux.h"

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
  
  game g = malloc (sizeof(struct game_s));

  g->tab = malloc(DEFAULT_SIZE * DEFAULT_SIZE * (sizeof(square)));
  square tab[DEFAULT_SIZE *DEFAULT_SIZE] = {
        TREE,  EMPTY, EMPTY, EMPTY, EMPTY, TREE , TREE,  EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
        TREE,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, TREE,  TREE,  EMPTY, EMPTY, TREE,  TREE , EMPTY};

  //each squares are initialized at the correct place
  for (int i = 0; i< DEFAULT_SIZE * DEFAULT_SIZE ; i++){
    g->tab[i] = tab[i];
  }
  
  g->nb_tent_col_expected = malloc(DEFAULT_SIZE * sizeof(uint));
  g->nb_tent_row_expected = malloc(DEFAULT_SIZE * sizeof(uint));
  uint tents_row[DEFAULT_SIZE] = {1,2,1,2,2,0,1,3};
  uint tents_col[DEFAULT_SIZE] = {3,0,2,1,2,1,2,1};
  
  //each expected tents lines are initialized
  for (int i = 0; i< DEFAULT_SIZE; i++){
    g->nb_tent_col_expected[i] = tents_col[i];
    g->nb_tent_row_expected[i] = tents_row[i];
  }

  return g;
}

game game_default_solution(void)
// Creates the default game solution.
{
  game g = malloc (sizeof(struct game_s));

  g->tab = malloc(DEFAULT_SIZE * DEFAULT_SIZE * (sizeof(square)));
  square tab[DEFAULT_SIZE *DEFAULT_SIZE] = {
        TREE,  EMPTY, EMPTY, EMPTY, TENT,  TREE , TREE,  EMPTY,
        TENT,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TENT,  EMPTY,
        EMPTY, EMPTY, TENT,  EMPTY, EMPTY, EMPTY, TREE,  EMPTY,
        TREE,  EMPTY, TREE,  EMPTY, TENT,  EMPTY, TENT,  EMPTY,
        TENT,  EMPTY, TENT,  EMPTY, TREE,  EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TENT,  EMPTY, EMPTY,
        TENT,  TREE,  TREE,  TENT,  EMPTY, TREE,  TREE , TENT};

  //each squares are initialized at the correct place
  for (int i = 0; i< DEFAULT_SIZE * DEFAULT_SIZE ; i++){
    g->tab[i] = tab[i];
  }
  
  g->nb_tent_col_expected = malloc(DEFAULT_SIZE * sizeof(uint));
  g->nb_tent_row_expected = malloc(DEFAULT_SIZE * sizeof(uint));
  uint tents_row[DEFAULT_SIZE] = {1,2,1,2,2,0,1,3};
  uint tents_col[DEFAULT_SIZE] = {3,0,2,1,2,1,2,1};
  
  //each expected tents lines are initialized
  for (int i = 0; i< DEFAULT_SIZE; i++){
    g->nb_tent_col_expected[i] = tents_col[i];
    g->nb_tent_row_expected[i] = tents_row[i];
  }

  return g;
}

static const char *dir[]{
  "HERE",  
  "UP",
  "DOWN",
  "LEFT",
  "RIGHT",
  "UP_LEFT",
  "UP_RIGHT",
  "DOWN_LEFT",
  "DOWN_RIGHT"
};  

bool game_get_ajacent_square(cgame g, uint i, uint j, direction d, uint* pi_next, uint* pj_next)
// Gets the coordinate of the adjacent square in a given direction.
{

  return 1;
}

bool game_is_neigh(cgame g, uint i, uint j, square s, bool diag)
// Determines if a neighboring square has a given value
{
  test[] = {0,0,0,1,1,0,1,1,1,-1,-1,1,-1,-1,1,-1};
  for(int i= 0; i<9; i=i+2){
    if (game_get_square(g, i + test[i] * DEFAULT_SIZE, j + test[i+1] * DEFAULT_SIZE) == s) return 1;
  }
  return 0;
}

int main(void){

  game g = game_default();

  return 1;
}