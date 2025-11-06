#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"

// 1 = TRUE

int test_game_won()
{
    game g;
    int result;


    g = game_default_solution();
    if (game_won(g)!=1){
        game_delete(g);
        return 0;
    }
    game_delete(g);
    // Sur jeu vide
    g = game_new_empty();
    result = game_won(g);
    game_delete(g);
    if (result != 1)
    {
        return 0;
    }
    

    // Cas de base
    g = game_default();
    result = game_won(g);
    game_delete(g);
    if (result != 0)
    {
        return 0;
    }


    // Tentes adjacentes : horizontale
    g = game_default();
    game_set_square(g, 1, 0, TENT);
    game_set_square(g, 1, 1, TENT);
    result = game_won(g);
    if (result != 0)
    {
        return 0;
    }
    game_delete(g);

    // Tentes adjacentes : Verticale
    g = game_default();
    game_set_square(g, 0, 1, TENT);
    game_set_square(g, 1, 1, TENT);
    result = game_won(g);
    if (result != 0)
    {
        return 0;
    }
    game_delete(g);

    // Tentes adjacentes : Diagonale
    g = game_default();
    game_set_square(g, 1, 1, TENT);
    game_set_square(g, 2, 2, TENT);
    result = game_won(g);
    if (result != 0)
    {
        return 0;
    }
    game_delete(g);


    // Règle 2
    g = game_default_solution();
    game_set_square(g, 1, 0, GRASS);
    game_set_square(g, 0, 1, TENT);
    result = game_won(g);
    if (result != 0)
    {
        return 0;
    }
    game_delete(g);

    // Règle 3 : nb tente != nb arbre
    g = game_default_solution();
    game_set_square(g, 1, 1, TENT);
    result = game_won(g);
    if (result != 0)
    {
        return 0;
    }
    game_delete(g);

    // Sans doute à revoir
    g = game_default_solution();
    game_set_square(g, 0, 4, EMPTY);
    result = game_won(g);
    if (result != 0)
    {
        return 0;
    }
    game_delete(g);

    g = game_default_solution();
    game_set_square(g, 0, 4, GRASS);
    game_set_square(g, 0, 2, TENT);
    result = game_won(g);
    if (result != 0)
    {
        return 0;
    }
    game_delete(g);


    g = game_default_solution();
    game_set_square(g, 1, 1, TENT);
    if (game_won(g) != 0)
    {
        game_delete(g);
        return 0;
    }
    game_delete(g);

    g = game_default_solution();
    game_set_square(g, 0, 4, GRASS);
    game_set_square(g, 1, 1, TENT);
    if (game_won(g) != 0)
    {
        game_delete(g);
        return 0;
    }
    game_delete(g);


    g = game_default_solution();
    game_set_square(g,0,4,GRASS);
    result = game_won(g);
    if (result != 0)
    {
        return 0;
    }
    game_delete(g);

    g = game_new_empty();
    game_set_square(g,0,4,TENT);
    result = game_won(g);
    if (result != 0)
    {
        return 0;
    }
    game_delete(g);


    return 1;
}

int test_unique_game_fill_grass(game g)
{
    // Récup l'état initial du tableau
    int tab_init[DEFAULT_SIZE][DEFAULT_SIZE];
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            tab_init[i][j] = game_get_square(g, i, j);
        }
    }
    game_fill_grass(g);
    // Vérifie que les cases empty ont été remplacé par grass
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            if (tab_init[i][j] == EMPTY && game_get_square(g, i, j) != GRASS)
            {
                fprintf(stderr, "Erreur : une case EMPTY n'a pas été transformée en GRASS");
                return 0;
            }

            else if (tab_init[i][j] != EMPTY && tab_init[i][j] != game_get_square(g, i, j))
            {

                fprintf(stderr, "Erreur : une case a été modifié alors qu'il ne fallait pas");
                return 0;
            }
        }
    }
    return 1;
}

int test_game_fill_grass()
{
    // A tester : game vide, game normale, game remplie de tente, game remplie d'arbre, game gagnée
    game tab[5];
    tab[0] = game_new_empty();
    tab[1] = game_default();
    tab[2] = game_new_empty();
    tab[3] = game_new_empty();
    tab[4] = game_default_solution();
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            game_set_square(tab[2], i, j, TENT);
            game_set_square(tab[3], i, j, TREE);
        }
    }
    int result = 1;
    for (int i = 0; i < 5; i++)
    {

        if (test_unique_game_fill_grass(tab[i]) == 0)
        {
            result = 0;
        }
        game_delete(tab[i]);
    }
    return result;
}

int test_unique_game_restart(game g)
{
    int tab_init[DEFAULT_SIZE][DEFAULT_SIZE];
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            tab_init[i][j] = game_get_square(g, i, j);
        }
    }
    game_restart(g);
    // Vérifie que les cases ont été remplacé par empty sauf les arbres
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            if (tab_init[i][j] != TREE && game_get_square(g, i, j) != EMPTY)
            {
                fprintf(stderr, "Erreur : une case n'a pas été transformée en empty");
                return 0;
            }

            else if (tab_init[i][j] == TREE && game_get_square(g, i, j) != TREE)
            {

                fprintf(stderr, "Erreur : Un arbre a été supprimé");
                return 0;
            }
        }
    }
    return 1;
}

int test_game_restart()
{
    // A tester : game vide, game normale, game remplie de tente, game remplie d'arbre, game gagnante
    game tab[5];
    tab[0] = game_new_empty();
    tab[1] = game_default();
    tab[2] = game_new_empty();
    tab[3] = game_new_empty();
    tab[4] = game_default_solution();
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            game_set_square(tab[2], i, j, TENT);
            game_set_square(tab[3], i, j, TREE);
        }
    }

    int result = 1;
    for (int i = 0; i < 5; i++)
    {

        if (test_unique_game_restart(tab[i]) == 0)
        {
            result = 0;
        }
        game_delete(tab[i]);
    }
    return result;
}

int test_game_is_neigh()
{
    game g_default = game_default(); // valide la condition (ici un arbre dans l'entourage de ..)
    game g_solution = game_default_solution();

    // Arbre adjacent (2,2)
    if (game_is_neigh(g_default, 2, 2, TREE, false) != 1)
    {
        fprintf(stderr, "Erreur : Vérifie en (2,1), renvoie false alors qu'il y a un arbre en (3,1)");
        game_delete(g_default);
        return 0;
    }
    // Arbre en diagonale
    if (game_is_neigh(g_default, 2, 3, TREE, true) != 1)
    {

        fprintf(stderr, "Erreur : Vérifie en (1,1), renvoie false alors qu'il y a un arbre en diagonale (1,1)");
        game_delete(g_default);
        return 0;
    }
    // si il y a un arbre uniquement en diagonale
    if (game_is_neigh(g_default, 2, 3, TREE, false) != 0)
    {
        fprintf(stderr, "Erreur : Vérifie en (2,3), renvoie true alors qu'il n'y a pas d'arbre (uniquement en diagonale)");
        game_delete(g_default);
        return 0;
    }
    // pas d'abre autours
    if (game_is_neigh(g_default, 5, 7, TREE, false) != 0)
    {
        fprintf(stderr, "Erreur : Vérifie en (5,7), renvoie true alors qu'il n'y a pas d'arbre en voisinnage");
        game_delete(g_default);
        return 0;
    }
    // Pas d'abre autours meme en diagonale
    if (game_is_neigh(g_default, 5, 7, TREE, true) != 0)
    {
        fprintf(stderr, "Erreur : Vérifie en (5,7), renvoie true alors qu'il n'y a pas d'abre en voisinnage (diagonale)");
        game_delete(g_default);
        return 0;
    }
    // en (0,0), sur un arbre mais rien autours
    if (game_is_neigh(g_default, 0, 0, TREE, true) != 0)
    {
        fprintf(stderr, "Erreur : Vérifie en (0,0), renvoie true alors qu'il n'y a pas d'abre en voisinnage");
        game_delete(g_default);
        return 0;
    }
    if (game_is_neigh(g_solution, 1, 1, TENT, true) != 1)
    {
        fprintf(stderr, "Erreur : n'a pas trouvé ses voisins tente");
        game_delete(g_solution);
        return 0;
    }
    if (game_is_neigh(g_default, 1, 1, TENT, false) != 0)
    {
        fprintf(stderr, "Erreur : A trouvé un voisin tente alors qu'il ne devrait pas");
        game_delete(g_solution);
        return 0;
    }
    // en (0,0), sur un arbre mais rien autours
    if (game_is_neigh(g_solution, 0, 3, TENT, false) != 1)
    {
        fprintf(stderr, "Erreur : n'a pas trouvé son voisin tente");
        game_delete(g_solution);
        return 0;
    }

    game_delete(g_default);
    return 1;
}

int test_game_default()
{
    game g = game_default();
    int arbre_place[12][2] = {{0, 0}, {0, 5}, {0, 6}, {2, 6}, {3, 0}, {3, 2}, {4, 4}, {5, 2}, {7, 1}, {7, 2}, {7, 5}, {7, 6}};
    if (game_get_expected_nb_tents_all(g) != 12){
        return 0;
    }
    //Vérifie si le nb de tente attendue sur chaque colonne est le bon
    if (game_get_expected_nb_tents_col(g,0)!=3 || game_get_expected_nb_tents_col(g,1)!=0 ||game_get_expected_nb_tents_col(g,2)!=2 ||game_get_expected_nb_tents_col(g,3)!=1 ||game_get_expected_nb_tents_col(g,4)!=2 ||game_get_expected_nb_tents_col(g,5)!=1 ||game_get_expected_nb_tents_col(g,6)!=2 ||game_get_expected_nb_tents_col(g,7)!=1){
        return 0;
    }
    if (game_get_expected_nb_tents_row(g,0)!=1 ||game_get_expected_nb_tents_row(g,1)!=2||game_get_expected_nb_tents_row(g,2)!=1||game_get_expected_nb_tents_row(g,3)!=2||game_get_expected_nb_tents_row(g,4)!=2||game_get_expected_nb_tents_row(g,5)!=0||game_get_expected_nb_tents_row(g,6)!=1||game_get_expected_nb_tents_row(g,7)!=3){
        return 0;
    }
    if (g == NULL)
    {
        fprintf(stderr, "Erreur : game_default retourne NULL \n");
        game_delete(g);
        return 0;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            int t = 0;
            for (int m = 0; m < 12; m++)
            {
                if (arbre_place[m][0] == i && arbre_place[m][1] == j)
                {
                    t = 1;
                }
            }
            if (t == 1 && game_get_square(g, i, j) != TREE)
            {
                game_delete(g);
                return 0;
            }
            if (t == 0 && game_get_square(g, i, j) != EMPTY)
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

int test_game_get_ajacent_square(){
    game g = game_default();
    game_print((cgame)g);
    uint pi_next;
    uint pj_next;
    if (game_get_ajacent_square(g,0,0,UP_LEFT, &pi_next, &pj_next) != 0){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,DEFAULT_SIZE-1,DEFAULT_SIZE-1,DOWN_RIGHT, &pi_next, &pj_next) != 0){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,1,1,UP_LEFT, &pi_next, &pj_next) != 1 || pi_next != 0 || pj_next !=0){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,3,3,DOWN, &pi_next, &pj_next) != 1 || pi_next != 4 || pj_next !=3){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,3,3,DOWN_RIGHT, &pi_next, &pj_next) != 1 || pi_next != 4 || pj_next !=4){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,3,3,RIGHT, &pi_next, &pj_next) != 1 || pi_next != 3 || pj_next !=4){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,3,3,UP_RIGHT, &pi_next, &pj_next) != 1 || pi_next != 2 || pj_next !=4){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,3,3,UP, &pi_next, &pj_next) != 1 || pi_next != 2 || pj_next !=3){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,3,3,UP_LEFT, &pi_next, &pj_next) != 1 || pi_next != 2 || pj_next !=2){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,3,3,LEFT, &pi_next, &pj_next) != 1 || pi_next != 3 || pj_next !=2){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,3,3,DOWN_LEFT, &pi_next, &pj_next) != 1 || pi_next != 4 || pj_next !=2){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,2,2,HERE, &pi_next, &pj_next) != 1 || pi_next != 2 || pj_next !=2){
        game_delete(g);
        return 0;
    }
    if (game_get_ajacent_square(g,0,3,UP, &pi_next, &pj_next) != 0){
        game_delete(g);
        return 0;
    }
    return 1;
}

int test_game_get_expected_nb_tents_all(){
    // TESTS SUR : un empty, en additionnant le nombre voulu pour chaque ligne et chaque colonne

    game g_empty = game_new_empty();
    game g = game_default();
    if (game_get_expected_nb_tents_all(g_empty) != 0){
        game_delete(g_empty);
        return 0; 
    }

    int a = 0;
    for (int i=0; i<DEFAULT_SIZE; i++){
        a = a + game_get_expected_nb_tents_col(g,i);
    }

    if (a != game_get_expected_nb_tents_all(g)){
        game_delete(g);
        return 0;
    }

    game_delete(g_empty);
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
    if (strcmp("game_get_ajacent_square", argv[1]) == 0)
    {
        ok = test_game_get_ajacent_square();
    }
    if (strcmp("game_get_expected_nb_tents_all", argv[1]) == 0)
    {
        ok = test_game_get_expected_nb_tents_all();
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