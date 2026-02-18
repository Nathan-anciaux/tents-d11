#include "game_tools.h"

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

static char square_to_char(square s)
{
  char c;
  switch (s) {
    case EMPTY:
      c = 'E';
      return c;
    case TREE:
      c = 'X';
      return c;
    case GRASS:
      c = 'G';
      return c;
    case BIGTREE:
      c = 'B';
      return c;
    case TENT:
      c = 'T';
      return c;
    default:
      return 'E';
  }
}

static char char_to_square(char c)
{
  square s;
  switch (c) {
    case 'E':
      s = EMPTY;
      return s;
    case 'G':
      s = GRASS;
      return s;
    case 'T':
      s = TENT;
      return s;
    case 'B':
      s = BIGTREE;
      return s;
    case 'X':
      s = TREE;
      return s;
    default:
      return EMPTY;
  }
}

game game_load(char* filename)
{
  FILE* file = fopen(filename, "r");

  // if NULL we try to put "saves/" in front of the path
  if (file == NULL) {
    char path_saves[512];
    snprintf(path_saves, sizeof(path_saves), "saves/%s", filename);
    file = fopen(path_saves, "r");
  }
  // else
  if (file == NULL) {
    // fprintf(stderr, "Fichier inexistant %s\n", filename);
    return NULL;
  }

  // we check the row and col and wrapp
  uint row, col, wrap;
  fscanf(file, "%u %u %u\n", &row, &col, &wrap);

  uint nb_row[row];
  uint nb_col[col];
  uint tmp;

  for (uint i = 0; i < row; i++) {
    fscanf(file, "%u ", &tmp);
    nb_row[i] = tmp;
  }
  fscanf(file, "\n");

  for (uint i = 0; i < col; i++) {
    fscanf(file, "%u ", &tmp);
    nb_col[i] = tmp;
  }
  fscanf(file, "\n");
  char char_tmp;

  // every square in a tab
  square squares[col * row];
  for (uint r = 0; r < row; r++) {
    for (uint c = 0; c < col; c++) {
      if (fscanf(file, " %c", &char_tmp) == 1) {
        squares[r * col + c] = char_to_square(char_tmp);
      }
    }
    fscanf(file, "\n");
  }

  fclose(file);
  // creating the game
  game g = game_new_ext(row, col, squares, nb_row, nb_col, wrap);

  return g;
}

void game_save(cgame g, char* filename)
{
  FILE* file = fopen(filename, "w");
  if (file == NULL) return;  // la sauvegarde a échoué

  // champs de game
  fprintf(file, "%u %u %u\n", g->nb_rows, g->nb_cols, g->wrapping);

  // row and col
  for (uint i = 0; i < g->nb_rows; i++) {
    fprintf(file, "%u ", game_get_expected_nb_tents_row(g, i));
  }
  fprintf(file, "\n");

  for (uint i = 0; i < g->nb_cols; i++) {
    fprintf(file, "%u ", game_get_expected_nb_tents_col(g, i));
  }
  fprintf(file, "\n");

  // copy of every game's square with a space between each other
  for (uint row = 0; row < g->nb_rows; row++) {
    for (uint col = 0; col < g->nb_cols; col++) {
      char c = game_get_square(g, row, col);
      fprintf(file, "%c ", square_to_char(c));
    }
    fprintf(file, "\n");
  }
  fclose(file);
}

int DIR2OFFSET2[][2] = {
    [HERE] = {0, 0},      [UP] = {-1, 0},       [DOWN] = {1, 0},       [LEFT] = {0, -1},      [RIGHT] = {0, 1},
    [UP_LEFT] = {-1, -1}, [UP_RIGHT] = {-1, 1}, [DOWN_LEFT] = {1, -1}, [DOWN_RIGHT] = {1, 1},
};

static bool game_check_move_special(cgame g, uint i, uint j, square s)
{
  assert(g);
  assert(s == EMPTY || s == TREE || s == TENT || s == GRASS || s == BIGTREE);

  if (i >= g->nb_rows) return false;
  if (j >= g->nb_cols) return false;

  square cs = SQUARE(g, i, j);  // current square at direction (i,j)

  if (s == TREE) return false;
  if (cs == TREE) return false;
  if (s == BIGTREE) return false;
  if (cs == BIGTREE) return false;

  if (game_nb_neigh(g, i, j, TENT, true) != 0) {
    return false;
  }

  return true;
}

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_trees, uint nb_bigtrees, bool solution)
{
  // On créé une game finie, si solution est true on la retourne sinon on retourne juste les arbres
  // Chaque itération on essaie de placer un tree ou un bigtree à un endroit aléatoire
  // Une fois qu'un arbre est placé, on place les tentes autours de lui, on check si le move est possible
  // (game_check_move) et on vérifie la victoire a chaque fois. Si c'est une victoire, on place l'arbre suivant.

  // On place d'abord les bigtree puis les tree. Si on ne peut pas bien placer la/les tentes, on supprime l'arbre et on
  // passe au suivant

  game g = game_new_empty_ext(nb_rows, nb_cols, wrapping);
  game_fill_grass(g);

  int max_try = nb_rows * nb_cols * 50;

  int try = 0;

  int nb_direction = 4;

  while (nb_bigtrees > 0 && try < max_try) {  // tant que il reste des big tree a placer
    try++;
    // game g_copy = game_copy(g);  // Sauvegarde de la game pour la restaurer si c'est pas valable

    //-- Place le Bigtree
    uint i = (((double)rand() * nb_rows) / RAND_MAX);  // On récupère des co aléatoires
    uint j = (((double)rand() * nb_cols) / RAND_MAX);

    if (game_get_square(g, i, j) != GRASS) continue;

    game_set_square(g, i, j, BIGTREE);  // On met le bigtreee

    //-- Place la première tente
    uint d1 = rand() % nb_direction;  // On prend une direction aléatoire

    int ii1 = i + DIR2OFFSET2[d1][0];
    int jj1 = j + DIR2OFFSET2[d1][1];  // On converti la direction en coordonnées ii1 et jj1

    // On vérifie les conditions pour pouvoir jouer
    if (wrapping) {
      ii1 = (ii1 % nb_rows + nb_rows) % nb_rows;
      jj1 = (jj1 % nb_cols + nb_cols) % nb_cols;
    } else {
      if ((ii1 < 0 || ii1 >= (int)g->nb_rows) || (jj1 < 0 || jj1 >= (int)g->nb_cols) ||
          game_get_square(g, ii1, jj1) != GRASS) {
        game_set_square(g, i, j, GRASS);
        continue;
      }
    }
    if (game_check_move_special(g, ii1, jj1, TENT) &&
        (game_get_square(g, ii1, jj1) == GRASS)) {  // Si c'est légal on joue la tente

      game_play_move(g, ii1, jj1, TENT);
    } else {
      game_set_square(g, i, j, GRASS);
      continue;
    }
    //-- On place la seconde tente
    uint d2;

    if (d1 == UP)
      d2 = DOWN;
    else if (d1 == DOWN)
      d2 = UP;
    else if (d1 == LEFT)
      d2 = RIGHT;
    else
      d2 = LEFT;

    int ii2 = i + DIR2OFFSET2[d2][0];
    int jj2 = j + DIR2OFFSET2[d2][1];  // On converti la direction en coordonnées ii1 et jj1

    // On vérifie les conditions pour pouvoir jouer
    if (wrapping) {
      ii2 = (ii2 % nb_rows + nb_rows) % nb_rows;
      jj2 = (jj2 % nb_cols + nb_cols) % nb_cols;
    } else {
      if ((ii2 < 0 || ii2 >= (int)g->nb_rows) || (jj2 < 0 || jj2 >= (int)g->nb_cols) ||
          game_get_square(g, ii2, jj2) != GRASS) {
        game_undo(g);  // supprime la première tente
        game_set_square(g, i, j, GRASS);
        continue;
      }
    }
    if (game_check_move_special(g, ii2, jj2, TENT) &&
        (game_get_square(g, ii2, jj2) == GRASS)) {  // Si c'est légal on joue la tente, c'est que les 2 sont valides ->
                                                    // on ajoute le nombre de tentes voulues
      game_play_move(g, ii2, jj2, TENT);
      game_set_expected_nb_tents_col(g, jj1, game_get_expected_nb_tents_col(g, jj1) + 1);
      game_set_expected_nb_tents_row(g, ii1, game_get_expected_nb_tents_row(g, ii1) + 1);
      game_set_expected_nb_tents_col(g, jj2, game_get_expected_nb_tents_col(g, jj2) + 1);
      game_set_expected_nb_tents_row(g, ii2, game_get_expected_nb_tents_row(g, ii2) + 1);
      nb_bigtrees--;
      try = 0;
    } else {
      game_undo(g);  // supprime la première tente
      game_set_square(g, i, j, GRASS);
      continue;
    }
  }
  try = 0;

  while (nb_trees > 0 && try < max_try) {  // tant que il reste des big tree a placer
    try++;

    //-- Place le tree
    uint i = (((double)rand() * nb_rows) / RAND_MAX);  // On récupère des co aléatoires
    uint j = (((double)rand() * nb_cols) / RAND_MAX);

    if (game_get_square(g, i, j) != GRASS) continue;

    game_set_square(g, i, j, TREE);  // On met le bigtreee

    //-- Place la première tente
    uint d1 = rand() % nb_direction;  // On prend une direction aléatoire

    int i_offset = DIR2OFFSET2[d1][0];
    int j_offset = DIR2OFFSET2[d1][1];
    int ii1 = i + i_offset;
    int jj1 = j + j_offset;  // On converti la direction en coordonnées ii1 et jj1

    // On vérifie les conditions pour pouvoir jouer
    if (wrapping) {
      ii1 = (ii1 % nb_rows + nb_rows) % nb_rows;
      jj1 = (jj1 % nb_cols + nb_cols) % nb_cols;
    } else {
      if ((ii1 < 0 || ii1 >= (int)g->nb_rows) || (jj1 < 0 || jj1 >= (int)g->nb_cols) ||
          game_get_square(g, ii1, jj1) != GRASS) {
        game_set_square(g, i, j, GRASS);
        continue;
      }
    }
    if (game_check_move_special(g, ii1, jj1, TENT) &&
        game_get_square(g, ii1, jj1) == GRASS) {  // Si c'est légal on joue la tente, c'est que les 2 sont valides -> on
                                                  // ajoute le nombre de tentes voulues
      game_play_move(g, ii1, jj1, TENT);
      game_set_expected_nb_tents_col(g, jj1, game_get_expected_nb_tents_col(g, jj1) + 1);
      game_set_expected_nb_tents_row(g, ii1, game_get_expected_nb_tents_row(g, ii1) + 1);
      nb_trees--;
      try = 0;
    } else {
      game_set_square(g, i, j, GRASS);
      continue;
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

int NB_DIR = 8;

int DIR2OFFSET2_solve[8][2] = {
    {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1},
};

// --------------------------------------
// Version rapide mais pas finie de solve (il manque les bigtree), la logique est bonne ça marche

/*bool game_solve_rec(game g, int pos, int tree_co[][2], int nb_tree)
{
  // printf("pos : %d, nb_tree : %d\n", pos, nb_tree);
  if (pos >= nb_tree) {
    if (game_won(g)) {
      printf("la game est won\n");
      return true;
    }

    else {
      printf("la game pas est won\n");
      return false;
    }
  }

  int i = tree_co[pos][0];  // indice ligne
  int j = tree_co[pos][1];  // indice colonne
  // printf("tente en : i : %d, j : %d\n", i, j);
  //  Si il n'y a pas de tente, on place
  if (game_nb_neigh(g, i, j, TENT, true) == 0) {
    // On parcours toute les directions
    for (int d = 0; d < NB_DIR; d++) {
      printf("d : %d\n", d);
      // On récupère les coordonnées ou potentiellement poser la tente
      int ii = (i + DIR2OFFSET2_solve[d][0] + game_nb_rows(g)) % game_nb_rows(g);
      int jj = (j + DIR2OFFSET2_solve[d][1] + game_nb_cols(g)) % game_nb_cols(g);

      printf("on place en : ii : %d, jj : %d\n", ii, jj);
      //  Si il n'y a pas d'arbre sur ces coordonnées ou de tente à coté et qu'il reste de la place dans la colonne /
      //  ligne
      if (game_get_square(g, ii, jj) != TREE && game_get_square(g, ii, jj) != TENT &&
          game_nb_neigh(g, ii, jj, TENT, true) == 0) {
        // On place la tente
        game_set_square(g, ii, jj, TENT);
        game_print(g);
        // Si la game est gagnante, on arrete
        if (game_solve_rec(g, pos + 1, tree_co, nb_tree)) {
          printf("gagnante !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
          game_print(g);
          return true;
        }

        // On met de l'herbe et on commence une nouvelle branche
        game_set_square(g, ii, jj, GRASS);
      }
    }
  } else {
    // Si il y a deja une tente, on avance
    return game_solve_rec(g, pos + 1, tree_co, nb_tree);
  }
  return false;
}

bool game_solve(game g)
{
  // Pour moi on compte d'abord le nombre de tentes voulues puis on récupère les coordonnées de chaque arbre. Ensuiteon
  // bruteforce toutes les possibilité autours des arbres.
  game gc = game_copy(g);  // On copie g pour ne pas la modifier
  game_fill_grass(g);
  int nb_row = game_nb_rows(g);
  int nb_col = game_nb_cols(g);
  int nb_game_win = 0;
  int nb_tents_wanted = 0;  // Récupère le nombre de tentes voulues en tout
  for (int i = 0; i < nb_col; i++) {
    nb_tents_wanted = nb_tents_wanted + game_get_expected_nb_tents_col(g, i);
  }
  int nb_tree = 0;  // on récupère le nombre total d'arbre et de bigtree pour pouvoir constuire notre liste
  int nb_bigtree = 0;
  for (int i = 0; i < nb_row; i++) {
    for (int j = 0; j < nb_col; j++) {
      if (game_get_square(g, i, j) == TREE) {
        nb_tree++;
      }
      if (game_get_square(g, i, j) == BIGTREE) {
        nb_bigtree++;
      }
    }
  }
  // On récupère les coordonnées de tous les tree et tous les bigtree
  int tree_co[nb_tree][2];
  int tree_co_index = 0;
  int bigtree_co[nb_bigtree][2];
  int bigtree_co_index = 0;
  for (int i = 0; i < nb_row; i++) {
    for (int j = 0; j < nb_col; j++) {
      if (game_get_square(g, i, j) == TREE) {
        tree_co[tree_co_index][0] = i;
        tree_co[tree_co_index][1] = j;
        tree_co_index++;
      }
      if (game_get_square(g, i, j) == BIGTREE) {
        bigtree_co[bigtree_co_index][0] = i;
        bigtree_co[bigtree_co_index][1] = j;
        bigtree_co_index++;
      }
    }
  }
  return game_solve_rec(g, 0, tree_co, nb_tree);
}
*/
// Le reste marche bof, c'est du brutforce pur

// uint game_nb_solutions_rec(game g, int pos, int* nb_sol)
// {
//   int i = pos / game_nb_cols(g);  // indice ligne
//   int j = pos % game_nb_cols(g);  // indice colonne

//   if ((i == game_nb_rows(g) && i == game_nb_cols(g)) ||
//       game_get_current_nb_tents_all(g) >= game_get_expected_nb_tents_all(g)) {
//     if (game_won(g)) {
//       game_print(g);
//       (*nb_sol)++;
//       return true;
//     }
//     return false;
//   }

//   bool a = false;
//   bool b = false;

//   if (((game_get_square(g, i, j) != TREE && game_get_square(g, i, j) != BIGTREE)) &&
//       game_get_current_nb_tents_col(g, j) < game_get_expected_nb_tents_col(g, j) &&
//       game_get_current_nb_tents_row(g, i) < game_get_expected_nb_tents_row(g, i)) {
//     game_set_square(g, i, j, TENT);
//     a = game_nb_solutions_rec(g, pos + 1, nb_sol);

//     game_set_square(g, i, j, GRASS);
//     b = game_nb_solutions_rec(g, pos + 1, nb_sol);
//   } else {
//     a = game_nb_solutions_rec(g, pos + 1, nb_sol);
//   }
//   return a || b;
// }

// uint game_nb_solutions(cgame g)
// {
//   int nb_sol = 0;
//   bool res = game_nb_solutions_rec(g, 0, &nb_sol);
//   printf("%d solutions\n", nb_sol);
//   return nb_sol;
// }

// bool game_solve_rec(game g, int pos, int* nb_try)
// {
//   // game_print(g);
//   (*nb_try)++;
//   int i = pos / game_nb_cols(g);  // indice ligne
//   int j = pos % game_nb_cols(g);  // indice colonne

//   if ((pos == game_nb_rows(g) * game_nb_cols(g)) ||
//       game_get_current_nb_tents_all(g) >= game_get_expected_nb_tents_all(g)) {
//     game_fill_grass(g);
//     if (game_won(g)) {
//       return true;
//     }
//     return false;
//   }

//   bool a = false;
//   bool b = false;
//   if (((game_get_square(g, i, j) != TREE && game_get_square(g, i, j) != BIGTREE)) &&
//       game_get_current_nb_tents_col(g, j) < game_get_expected_nb_tents_col(g, j) &&
//       game_get_current_nb_tents_row(g, i) < game_get_expected_nb_tents_row(g, i)) {
//     game_set_square(g, i, j, TENT);
//     a = game_solve_rec(g, pos + 1, nb_try);
//     if (a) return a;
//     game_set_square(g, i, j, GRASS);
//     b = game_solve_rec(g, pos + 1, nb_try);
//     if (b) return b;
//   } else {
//     a = game_solve_rec(g, pos + 1, nb_try);
//   }
//   return a || b;
// }

// bool game_solve(game g)
// {
//   int nb_appel = 0;
//   bool res = game_solve_rec(g, 0, &nb_appel);
//   printf("%d appels\n", nb_appel);
//   return res;
// }
