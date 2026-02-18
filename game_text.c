#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

void print_help(void)
{
  printf(
      "Liste commandes : \nt x y : mettre une tente en (x,y)\ng x y : mettre "
      "de l'herbe en (x,y)\ne x y : supprimer le contenu en (x,y)\nr = "
      "restart\nf = remplir avec herbe\ns = sauvegarder la partie\nq = quitter \n");

  return;
}

int main(int argc, char* argv[])
{
  // square squares[18] = {EMPTY, TREE,  EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  //                       EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY};
  // uint nb_row[6] = {1, 0, 0, 0, 0, 0};
  // uint nb_col[3] = {1, 0, 0};
  // game g = game_new_ext(6, 3, squares, nb_row, nb_col, true);
  // srand(time(NULL));
  // game g = game_random(6, 6, 1, 2, 0, 0);
  // game_print(g);
  // bool gs = game_solve(g);
  // // game_print(g);
  // printf("la game a une solution : %d\n", gs);

  if (strstr(argv[0], "game_random") != NULL) {
    if (argc < 7) {
      return EXIT_FAILURE;
    }

    game g = game_random(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]), atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));
    if (argc == 8) {
      printf("sauvegarde\n");
      game_save(g, argv[7]);
    }
    game_print(g);
    game_delete(g);
  } else {
    char file[200];
    if (argc > 1) {
      sprintf(file, "%s", argv[1]);
    } else {
      sprintf(file, "default.txt");
    }

    game g = game_load(file);
    if (g == NULL) {
      printf("Erreur !\n");
      return EXIT_FAILURE;
    }
    game_print((cgame)g);

    char c;

    while (!game_won((cgame)g)) {
      if (scanf(" %c", &c) != 1) break;
      if (c == 'h') {
        print_help();
      }
      if (c == 't' || c == 'g' || c == 'e') {
        int j, i;
        scanf("%d %d", &j, &i);
        if (c == 't') {
          game_play_move(g, j, i, TENT);
        } else if (c == 'g') {
          game_play_move(g, j, i, GRASS);
        } else if (c == 'e') {
          game_play_move(g, j, i, EMPTY);
        }
      } else if (c == 'y') {
        game_redo(g);
      } else if (c == 'z') {
        game_undo(g);
      } else if (c == 'r') {
        game_restart(g);
      } else if (c == 'f') {
        game_fill_grass(g);
      } else if (c == 'q') {
        printf("shame \n");
        game_delete(g);
        return EXIT_SUCCESS;
      } else if (c == 's') {
        char tmp[200];
        char nom[205];
        scanf("%s", tmp);
        sprintf(nom, "%s", tmp);
        game_save(g, nom);
      }
      game_print((cgame)g);
    }
    if (game_won(g)) {
      printf("congratulation\n");
      game_delete(g);
    }
  }

  return EXIT_SUCCESS;
}