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
  game_fill_grass(g);

  direction* all_direcion = {UP, DOWN, LEFT, RIGHT};
  int nb_direction = 4;

  while (nb_bigtrees > 0) {      // tant que il reste des big tree a placer
    game g_copy = game_copy(g);  // Sauvegarde de la game pour la restaurer si c'est pas valable

    uint i = (((double)rand() * nb_rows) / RAND_MAX);  // On récupère des co aléatoires
    uint j = (((double)rand() * nb_cols) / RAND_MAX);

    game_set_square(g, i, j, BIGTREE);  // On met le bigtreee

    uint d1 = rand() % nb_direction;  // On prend une direction aléatoire

    int i_offset = DIR2OFFSET2[d1][0];
    int j_offset = DIR2OFFSET2[d1][1];
    int ii1 = i + i_offset;
    int jj1 = j + j_offset;  // On converti la direction en coordonnées ii1 et jj1

    if ((ii1 < 0 || ii1 >= (int)g->nb_rows) || (jj1 < 0 || jj1 >= (int)g->nb_cols) ||
        (game_get_square(g, ii1, jj1) !=
         GRASS)) {  // si les coordonnées sont en dehors des bornes ou que c'est pas sur un empty
      g = g_copy;
      continue;
    } else {
      game_set_expected_nb_tents_col(
          g, jj1,
          game_get_expected_nb_tents_col(g, jj1) + 1);  // On ajoute 1 au nombre de tente voulu la ou on l'a placé
      game_set_expected_nb_tents_row(g, ii1, game_get_expected_nb_tents_row(g, ii1) + 1);
      game_set_square(g, ii1, jj1, TENT);  // on place la tente
    }

    uint d2;  // 2 tentes ne peuvent pas être en diagonale
    if (d1 == UP) d2 = DOWN;
    if (d1 == DOWN) d2 = UP;
    if (d1 == LEFT) d2 = RIGHT;
    if (d1 == RIGHT) d2 = LEFT;

    int i_offset2 = DIR2OFFSET2[d2][0];
    int j_offset2 = DIR2OFFSET2[d2][1];
    int ii2 = i + i_offset2;
    int jj2 = j + j_offset2;  // coordonnées de la nouvelle

    if ((ii2 < 0 || ii2 >= (int)g->nb_rows) || (jj2 < 0 || jj2 >= (int)g->nb_cols) ||
        (game_get_square(g, ii2, jj2) != GRASS)) {  // si c'est en dehors des bornes ou égal aux premières coordonnées
      g = g_copy;
      continue;
    } else {
      game_set_expected_nb_tents_col(
          g, jj2,
          game_get_expected_nb_tents_col(g, jj2) + 1);  // On ajoute 1 au nombre de tente voulu la ou on l'a placé
      game_set_expected_nb_tents_row(g, ii2, game_get_expected_nb_tents_row(g, ii2) + 1);

      game_set_square(g, ii2, jj2, TENT);
    }

    if (!game_won(g)) {  // SI la game n'est pas gagnante, on nettoie tout
      g = g_copy;
      continue;
    } else {
      nb_bigtrees--;
    }
  }
  while (nb_trees > 0) {         // tant que il reste des big tree a placer
    game g_copy = game_copy(g);  // Sauvegarde de la game pour la restaurer si c'est pas valable

    uint i = (((double)rand() * nb_rows) / RAND_MAX);  // On récupère des co aléatoires
    uint j = (((double)rand() * nb_cols) / RAND_MAX);

    game_set_square(g, i, j, TREE);  // On met le bigtreee

    uint d1 = rand() % nb_direction;  // On prend une direction aléatoire

    int i_offset = DIR2OFFSET2[d1][0];
    int j_offset = DIR2OFFSET2[d1][1];
    int ii1 = i + i_offset;
    int jj1 = j + j_offset;  // On converti la direction en coordonnées ii1 et jj1

    if ((ii1 < 0 || ii1 >= (int)g->nb_rows) || (jj1 < 0 || jj1 >= (int)g->nb_cols) ||
        (game_get_square(g, ii1, jj1) !=
         GRASS)) {  // si les coordonnées sont en dehors des bornes ou que c'est pas sur un empty
      g = g_copy;
      continue;
    } else {
      game_set_expected_nb_tents_col(
          g, jj1,
          game_get_expected_nb_tents_col(g, jj1) + 1);  // On ajoute 1 au nombre de tente voulu la ou on l'a placé
      game_set_expected_nb_tents_row(g, ii1, game_get_expected_nb_tents_row(g, ii1) + 1);
      game_set_square(g, ii1, jj1, TENT);  // on place la tente
    }

    if (!game_won(g)) {  // SI la game n'est pas gagnante, on nettoie tout
      g = g_copy;
      continue;
    } else {
      nb_trees--;
    }
  }

  if (solution) {
    return g;
  } else {
    for (int i = 0; i < nb_rows; i++) {
      for (int j = 0; j < nb_cols; j++) {
        if (game_get_square(g, i, j) != TREE && game_get_square(g, i, j) != BIGTREE) {
          game_set_square(g, i, j, EMPTY);
        }
      }
    }
  }
  return g;
}

// On place un bigtree sur une case ou y'a rien. On place la première tente à une co aléatoire autours de lui VIDE. on
// place une deuxième tente aléatoie autours de lui la ou c'est VIDE. On ajoute le nb_tent_expected. Si la game est
// gagnant, on continue, sinon on clear tout