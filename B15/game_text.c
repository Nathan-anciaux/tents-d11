#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_struct.h"
#include "queue.h"

int main(void) {
  game g = game_default();
  game_print(g);
  while (game_won(g) == false) {
    printf("\n> ? [h for help]\n");
    char c;
    scanf(" %c", &c);
    if (c == 'h') {
      printf("\n't', mettre une pièce TENT dans la case (<i>,<j>)\n");
      printf("'g', mettre la pièce GRASS dans la case (<i>,<j>)\n");
      printf("'e', mettre la pièce EMPTY dans la case (<i>,<j>)\n");
      printf("'r', reinitialiser la grille\n");
      printf("'f', remplir la grille avec de l'herbe\n");
      printf("'z', annuler le dernier coup (undo)\n");
      printf("'y', rétablir le dernier coup annulé (redo)\n");
      printf("'q', quitter\n\n");
      continue;
    } else if (c == 't' || c == 'g' || c == 'e') {
      int i, j;
      scanf(" %d %d", &i, &j);

      if (c == 't' && game_check_move(g, i, j, TENT)) {
        game_play_move(g, i, j, TENT);
      } else if (c == 'g' && game_check_move(g, i, j, GRASS)) {
        game_play_move(g, i, j, GRASS);
      } else if (c == 'e' && game_check_move(g, i, j, EMPTY)) {
        game_play_move(g, i, j, EMPTY);
      }
    }

    else if (c == 'r') {
      game_restart(g);
    }

    else if (c == 'f') {
      game_fill_grass(g);
    } else if (c == 'z') {
      game_undo(g);
    } else if (c == 'y') {
      game_redo(g);
    } else if (c == 'q') {
      break;
    }
    game_print(g);
  }

  if (game_won(g)) {
    printf("congratulation\n");
  } else {
    printf("shame\n");
  }

  game_delete(g);
  return EXIT_SUCCESS;
}
