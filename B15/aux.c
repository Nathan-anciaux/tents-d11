#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_ext.h"
#include "game_struct.h"

game game_default_ext(void) {
  // 🟩🌲🟩🟩⛺ 1
  // 🟩⛺🟩🟩🟩 1
  // 🟩🟩🟩🟩⛺ 1
  // 🟩⛺🌲🟩🌳 1
  //  0 2  0 0 2

  game g = game_new_empty_ext(4, 5, true);
  assert(g != NULL);
  game_set_square(g, 0, 1, TREE);
  game_set_square(g, 3, 2, TREE);
  game_set_square(g, 3, 4, BIGTREE);
  game_set_expected_nb_tents_col(g, 0, 0);
  game_set_expected_nb_tents_col(g, 1, 2);
  game_set_expected_nb_tents_col(g, 2, 0);
  game_set_expected_nb_tents_col(g, 3, 0);
  game_set_expected_nb_tents_col(g, 4, 2);
  game_set_expected_nb_tents_row(g, 0, 1);
  game_set_expected_nb_tents_row(g, 1, 1);
  game_set_expected_nb_tents_row(g, 2, 1);
  game_set_expected_nb_tents_row(g, 3, 1);
  return g;
}

game game_default_solution_ext(void) {
  game g = game_default_ext();
  if (!g) {
    fprintf(stderr, "g NULL\n");
    exit(1);
  }
  if (!g->grille) {
    fprintf(stderr, "grille NULL\n");
    exit(1);
  }
  for (uint i = 0; i < g->height; i++)
    if (!g->grille[i]) {
      fprintf(stderr, "ligne %u NULL\n", i);
      exit(1);
    }
  g->grille[0][4] = TENT;
  g->grille[1][1] = TENT;
  g->grille[2][4] = TENT;
  g->grille[3][1] = TENT;
  game_fill_grass(g);
  return g;
}