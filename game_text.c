#include <stdlib.h>
#include <stdio.h>
#include "game.h"
#include "game_aux.h"

//

void print_help(void)
{
    printf("Liste commandes : \nt x y : mettre une tente en (x,y)\ng x y : mettre de l'herbe en (x,y)\ne x y : supprimer le contenu en (x,y)\nr = restart\nf  = remplir avec herbe\nq = quitter \n");

    return;
}

int main(void)
{
    game g = game_default();
    game_print((cgame)g);

    char c;

    while (!game_won((cgame)g))
    {
        scanf(" %c", &c);
        if (c == 'h')
        {
            print_help();
        }
        if (c == 't' || c == 'g' || c == 'e')
        {
            int j, i;
            scanf("%d %d", &j, &i);
            if (c == 't')
            {
                game_play_move(g, j, i, TENT);
            }
            else if (c == 'g')
            {
                game_play_move(g, j, i, GRASS);
            }
            else if (c == 'e')
            {
                game_play_move(g, j, i, EMPTY);
            }
        }
        else if (c == 'r')
        {
            game_restart(g);
        }
        else if (c == 'f')
        {
            game_fill_grass(g);
        }
        else if (c == 'q')
        {
            printf("shame \n");
            game_delete(g);
            return EXIT_SUCCESS;
        }
        game_print((cgame)g);
    }
    printf("congratulation\n");
    game_delete(g);
    return EXIT_SUCCESS;
}