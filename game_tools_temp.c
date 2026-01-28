#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_struct.h"
#include "game_tools.h"

int DIR2OFFSET2[][2] = {
    [HERE] = {0, 0},      [UP] = {-1, 0},       [DOWN] = {1, 0},       [LEFT] = {0, -1},      [RIGHT] = {0, 1},
    [UP_LEFT] = {-1, -1}, [UP_RIGHT] = {-1, 1}, [DOWN_LEFT] = {1, -1}, [DOWN_RIGHT] = {1, 1},
};

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_trees, uint nb_bigtrees, bool solution)
{
  // On créé une game finie, si solution est true on la retourne sinon on retourne juste les arbres
  // Chaque itération on essaie de placer un tree ou un bigtree à un endroit aléatoire
  // Une fois qu'un arbre est placé, on place les tentes autours de lui, on check si le move est possible
  // (game_check_move) et on vérifie la victoire a chaque fois. Si c'est une victoire, on place l'arbre suivant.

  // On place d'abord les bigtree puis les tree. Si on ne peut pas bien placer la/les tentes, on supprime l'arbre et on
  // passe au suivant
  printf("go !\n");
  srand(time(NULL));
  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);

  direction* all_direcion = {HERE, UP, DOWN, LEFT, RIGHT, UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT};
  int nb_direction = 9;

  while (nb_bigtrees > 0) {
    uint i = (((double)rand() * nb_rows) / RAND_MAX);
    uint j = (((double)rand() * nb_cols) / RAND_MAX);

    printf("i : %d, j : %d\n", i, j);
    game_set_square(g, i, j, BIGTREE);

    uint d1 = (((double)rand() * nb_direction) / RAND_MAX);
    int i_offset = DIR2OFFSET2[d1][0];
    int j_offset = DIR2OFFSET2[d1][1];
    int ii1 = i + i_offset;
    int jj1 = j + j_offset;
    if ((ii1 < 0 || ii1 >= (int)g->nb_rows) || (jj1 < 0 || jj1 >= (int)g->nb_cols)) {
      game_set_square(g, i, j, EMPTY);
      continue;
    } else {
      game_set_square(g, ii1, jj1, TENT);
    }

    uint d2 = (((double)rand() * nb_direction) / RAND_MAX);
    int i_offset2 = DIR2OFFSET2[d2][0];
    int j_offset2 = DIR2OFFSET2[d2][1];
    int ii2 = i + i_offset2;
    int jj2 = j + j_offset2;
    if (ii2 < 0 || ii2 >= (int)g->nb_rows) continue;
    if (jj2 < 0 || jj2 >= (int)g->nb_cols) continue;
    if ((ii2 < 0 || ii2 >= (int)g->nb_rows) || (jj2 < 0 || jj2 >= (int)g->nb_cols)) {
      game_set_square(g, ii1, jj1, EMPTY);
      continue;
    } else {
      game_set_square(g, ii2, jj2, TENT);
      printf("2 eme tente placée\n");
    }
    nb_bigtrees--;
    game_print(g);
    return g;
  }
  return g;
  //   while (nb_trees > 0) {
  //     uint i = (((double)rand() * nb_rows) / RAND_MAX);
  //     uint j = (((double)rand() * nb_cols) / RAND_MAX);
  //   }
}
