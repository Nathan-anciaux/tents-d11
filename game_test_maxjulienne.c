#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"

// 1 = TRUE

int test_game_won()
{
    // Ne marche pas...

    // A tester : jeu vide, jeu gagnant, jeu pas gagné
    game tab[3];
    tab[0] = game_new_empty();
    tab[1] = game_default_solution();
    tab[2] = game_default();
    game_print((cgame)tab[1]);
    printf("%d\n", game_won(tab[1]));

    // attendu 0
    if (game_won(tab[0]) != 1)
    {

        fprintf(stderr, "Erreur : jeu vide détecté comme gagnant");
        return 0;
    }
    // attendu 1
    if (game_won(tab[1]) != 1)
    {
        fprintf(stderr, "Erreur : jeu gagnant détecté comme non gagnant");
        return 0;
    }
    // attendu 0
    if (game_won(tab[2]) != 0)
    {
        printf("%d\n", game_won(tab[2]));
        fprintf(stderr, "Erreur : jeu non gagnant détecté comme gagnant");
        return 0;
    }

    for (int i = 0; i < 3; i++)
    {
        game_delete(tab[i]);
    }
    return 1;
}

int test_unique_game_fill_grass(game g)
{
    int tab_grass[DEFAULT_SIZE * DEFAULT_SIZE][2];
    int index_grass = 0;
    int tab_not_grass[DEFAULT_SIZE * DEFAULT_SIZE][2];
    int index_not_grass = 0;
    // récup tous les endroits ou c'est vide, et ceux ou ça ne l'est pas
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            if (game_get_square(g, i, j) == EMPTY)
            {
                tab_grass[index_grass][0] = i;
                tab_grass[index_grass][1] = j;
                index_grass++;
            }
            else
            {
                tab_not_grass[index_not_grass][0] = i;
                tab_not_grass[index_not_grass][1] = j;
                index_not_grass++;
            }
        }
    }
    game_fill_grass(g);
    // Vérifier si il y a bien de l'herbe la ou il y avait rien et que sinon rien a bougé
    for (int i = 0; i < index_grass; i++)
    {
        if (game_get_square(g, tab_grass[i][0], tab_grass[i][1]) != GRASS)
        {
            return 0;
        }
    }
    for (int i = 0; i < index_not_grass; i++)
    {
        if (game_get_square(g, tab_not_grass[i][0], tab_not_grass[i][1]) == GRASS)
        {
            return 0;
        }
    }
    return 1;
}

int test_game_fill_grass()
{
    // A tester : game vide, game normale, game remplie de tente, game remplie d'arbre
    game tab[4];
    tab[0] = game_new_empty();
    tab[1] = game_default();
    tab[2] = game_new_empty();
    tab[3] = game_new_empty();
    game_print(tab[1]);
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            game_set_square(tab[2], i, j, TENT);
            game_set_square(tab[3], i, j, TREE);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (test_unique_game_fill_grass(tab[i]) == 0)
        {
            game_delete(tab[i]);
            return 0;
        }
    }
    return 1;
}

int test_unique_game_restart(game g)
{
    int tab_tree[DEFAULT_SIZE * DEFAULT_SIZE][2];
    int index_tree = 0;
    int tab_not_tree[DEFAULT_SIZE * DEFAULT_SIZE][2];
    int index_not_tree = 0;
    // récup tous les endroits ou c'est des arbres, et ceux ou ça ne l'est pas
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            if (game_get_square(g, i, j) == TREE)
            {
                tab_tree[index_tree][0] = i;
                tab_tree[index_tree][1] = j;
                index_tree++;
            }
            else
            {
                tab_not_tree[index_not_tree][0] = i;
                tab_not_tree[index_not_tree][1] = j;
                index_not_tree++;
            }
        }
    }
    game_restart(g);
    // Vérifier si il y a bien des arbres la ou ils étaient et que tout s'est supprimé
    for (int i = 0; i < index_tree; i++)
    {
        if (game_get_square(g, tab_tree[i][0], tab_tree[i][1]) != TREE)
        {
            return 0;
        }
    }
    for (int i = 0; i < index_not_tree; i++)
    {
        if (game_get_square(g, tab_not_tree[i][0], tab_not_tree[i][1]) != EMPTY)
        {
            return 0;
        }
    }
    return 1;
}

int test_game_restart()
{
    // A tester : game vide, game normale, game remplie de tente, game remplie d'arbre
    game tab[4];
    tab[0] = game_new_empty();
    tab[1] = game_default();
    tab[2] = game_new_empty();
    tab[3] = game_new_empty();
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            game_set_square(tab[2], i, j, TENT);
            game_set_square(tab[3], i, j, TREE);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        if (test_unique_game_restart(tab[i]) == 0)
        {
            game_delete(tab[i]);
            return 0;
        }
    }
    return 1;
}

int test_game_is_neigh()
{
    // on sait qu'en (2,2) il y a un arbre juste en dessous en (3,1), aussi en (1,1) il y a un arbre uniquement en diagonale (0,0), en (5,7), il n'y a aucun voisin, tout est vide
    game g;
    g = game_default(); // valide la condition (ici un arbre dans l'entourage de ..)

    // Arbre adjacent (2,2)
    if (game_is_neigh(g, 2, 2, TREE, false) != 1)
    {
        fprintf(stderr, "Erreur : Vérifie en (2,1), renvoie false alors qu'il y a un arbre en (3,1)");
        return 0;
    }
    // Arbre en diagonale
    if (game_is_neigh(g, 2, 3, TREE, true) != 1)
    {

        fprintf(stderr, "Erreur : Vérifie en (1,1), renvoie false alors qu'il y a un arbre en diagonale (1,1)");
        return 0;
    }
    // si il y a un arbre uniquement en diagonale
    if (game_is_neigh(g, 2, 3, TREE, false) != 0)
    {
        fprintf(stderr, "Erreur : Vérifie en (2,3), renvoie true alors qu'il n'y a pas d'arbre (uniquement en diagonale)");
        return 0;
    }
    // pas d'abre autours
    if (game_is_neigh(g, 5, 7, TREE, false) != 0)
    {
        fprintf(stderr, "Erreur : Vérifie en (5,7), renvoie true alors qu'il n'y a pas d'arbre en voisinnage");
        return 0;
    }
    // Pas d'abre autours meme en diagonale
    if (game_is_neigh(g, 5, 7, TREE, true) != 0)
    {
        fprintf(stderr, "Erreur : Vérifie en (5,7), renvoie true alors qu'il n'y a pas d'abre en voisinnage (diagonale)");
        return 0;
    }
    // en (0,0), sur un arbre mais rien autours
    if (game_is_neigh(g, 0, 0, TREE, true) != 0)
    {
        fprintf(stderr, "Erreur : Vérifie en (0,0), renvoie true alors qu'il n'y a pas d'abre en voisinnage");
        return 0;
    }

    game_delete(g);
    return 1;
}

int test_game_default()
{
    game g = game_default();
    if (g == NULL)
    {
        fprintf(stderr, "Erreur : game_default retourne NULL \n");
        game_delete(g);
        return 0;
    }

    if (game_get_square(g, 0, 0) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 0, 5) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 0, 6) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 2, 6) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 3, 0) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 3, 2) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 4, 4) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 5, 2) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 7, 1) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 7, 2) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 7, 5) != TREE)
    {
        game_delete(g);
        return 0;
    }
    if (game_get_square(g, 7, 6) != TREE)
    {
        game_delete(g);
        return 0;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            if (game_get_square(g, i, j) != TREE && game_get_square(g, i, j) != EMPTY)
            {
                game_delete(g);
                return 0;
            }
        }
    }
    game_delete(g);
    return 1;
}

int test_game_default_solution()
{
    game g = game_default_solution();
    game_print((cgame)g);
    if (g == NULL)
    {
        fprintf(stderr, "Erreur : game_default retourne NULL \n");
        return 0;
    }
    int trees[][2] = {
        {0, 0}, {0, 5}, {0, 6}, {2, 6}, {3, 0}, {3, 2}, {4, 4}, {5, 2}, {7, 1}, {7, 2}, {7, 5}, {7, 6}};
    int size_trees = 12;
    int size_tents = 12;
    int tent[][2] = {
        {0, 4}, {1, 0}, {1, 6}, {2, 2}, {3, 4}, {3, 6}, {4, 0}, {4, 2}, {6, 5}, {7, 0}, {7, 3}, {7, 7}};
    int grass[][2] = {
        {0, 1}, {0, 2}, {0, 3}, {0, 7}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {1, 7}, {2, 0}, {2, 1}, {2, 3}, {2, 4}, {2, 5}, {2, 7}, {3, 1}, {3, 3}, {3, 5}, {3, 7}, {4, 1}, {4, 3}, {4, 5}, {4, 6}, {4, 7}, {5, 0}, {5, 1}, {5, 3}, {5, 4}, {5, 5}, {5, 6}, {5, 7}, {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 6}, {6, 7}, {7, 4}};
    int size_grass = 40;
    for (int i = 0; i < size_trees; i++)
    {
        if (game_get_square(g, trees[i][0], trees[i][1]) != TREE)
        {
            game_delete(g);
            return 0;
        }
    }
    for (int i = 0; i < size_tents; i++)
    {

        if (game_get_square(g, tent[i][0], tent[i][1]) != TENT)
        {
            game_delete(g);
            return 0;
        }
    }
    for (int i = 0; i < size_grass; i++)
    {
        if (game_get_square(g, grass[i][0], grass[i][1]) != GRASS)
        {
            game_delete(g);
            return 0;
        }
    }
    game_delete(g);
    return 1;
}

int test_game_print()
{
    game g = game_default();
    game_print((cgame)g);
    game_delete(g);
    return 1;
}

int test_dummy()
{
    return 1;
}

void usage(int argc, char *argv[])
{
    fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    if (argc == 1)
        usage(argc, argv);
    bool ok = 0;
    if (strcmp("dummy", argv[1]) == 0)
    {
        ok = test_dummy();
    }
    if (strcmp("game_won", argv[1]) == 0)
    {
        ok = test_game_won();
    }
    if (strcmp("game_fill_grass", argv[1]) == 0)
    {
        ok = test_game_fill_grass();
    }
    if (strcmp("game_restart", argv[1]) == 0)
    {
        ok = test_game_restart();
    }
    if (strcmp("game_is_neigh", argv[1]) == 0)
    {
        ok = test_game_is_neigh();
    }
    if (strcmp("game_default", argv[1]) == 0)
    {
        ok = test_game_default();
    }
    if (strcmp("game_default_solution", argv[1]) == 0)
    {
        ok = test_game_default_solution();
    }
    if (strcmp("game_print", argv[1]) == 0)
    {
        ok = test_game_print();
    }
    if (ok)
    {
        fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
        return EXIT_FAILURE;
    }
}