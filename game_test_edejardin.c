#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "game.h"
#include "game_aux.h"

bool test_game_new(void);
bool test_game_new_empty(void);
bool test_game_copy(void);
bool test_game_equal(void);
bool test_game_delete(void);
bool test_game_play_move(void);
bool test_game_check_move(void);
bool test_game_get_current_nb_tents_all(void);

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
    if (strcmp("game_new", argv[1]) == 0)
    {
        ok = test_game_new();
    }
    if (strcmp("game_new_empty", argv[1]) == 0)
    {
        ok = test_game_new_empty();
    }
    if (strcmp("game_copy", argv[1]) == 0)
    {
        ok = test_game_copy();
    }
    if (strcmp("game_equal", argv[1]) == 0)
    {
        ok = test_game_equal();
    }
    if (strcmp("game_delete", argv[1]) == 0)
    {
        ok = test_game_delete();
    }
    if (strcmp("game_play_move", argv[1]) == 0)
    {
        ok = test_game_play_move();
    }
    if (strcmp("game_check_move", argv[1]) == 0)
    {
        ok = test_game_check_move();
    }
    if (strcmp("game_get_current_nb_tents_all", argv[1]) == 0)
    {
        ok = test_game_get_current_nb_tents_all();
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

bool test_game_new(void)
{
    // je regarde sii defaut = new je regarde si en changeant les parametres il se passe quelque chose.
    uint nb_tents_row[DEFAULT_SIZE] = {1, 2, 1, 2, 2, 0, 1, 3};
    uint nb_tents_col[DEFAULT_SIZE] = {3, 0, 2, 1, 2, 1, 2, 1};
    square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
        TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, TREE, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY,
        TREE, EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, TREE, TREE, EMPTY, EMPTY, TREE, TREE, EMPTY};

    bool test = true;
    game g_new = game_new(squares, nb_tents_row, nb_tents_col);
    test = test && g_new != NULL; // regarde si ça a bien marché

    game g_ref = game_default();
    test = test && g_ref != NULL; // test qui est tjr vrai normalement

    test = test && game_equal(g_new, g_ref);

    // Le jeu g_new ne doit PAS changer (il doit avoir fait une copie).
    squares[0] = EMPTY;
    nb_tents_row[0] = 99;
    nb_tents_col[0] = 99;

    test = test && game_get_square(g_new, 0, 0) == TREE;          // Doit être resté TREE
    test = test && game_get_expected_nb_tents_row(g_new, 0) == 1; // Doit être resté 1
    test = test && game_get_expected_nb_tents_col(g_new, 0) == 3; // Doit être resté 3

    nb_tents_row[0] = 1;
    nb_tents_col[0] = 3;

    for (uint i = 0; i < DEFAULT_SIZE; i++)
    {
        test = test && nb_tents_col[i] == game_get_expected_nb_tents_col(g_new, i); // Regarde si le nombre de tentes est bon
        test = test && nb_tents_row[i] == game_get_expected_nb_tents_row(g_new, i); // Regarde si le nombre de tentes est bon
    }

    squares[0] = TREE;
    nb_tents_row[0] = 7;
    nb_tents_col[0] = 7;

    g_new = game_new(squares, nb_tents_row, nb_tents_col);
    test = test && g_new != NULL; // regarde si ça a bien marché

    g_ref = game_default();
    test = test && g_ref != NULL; // test qui est tjr vrai normalement

    test = test && !game_equal(g_new, g_ref);

    // Attention j'ai fait l'implementation naive qui va empecher les nombres abusés, mais faut faire nombre de empty /2 (+1) fin meme pas c'est plus subtil nsm
    for (uint i = 0; i < DEFAULT_SIZE; i++)
    {
        test = test && nb_tents_col[i] == game_get_expected_nb_tents_col(g_new, i); // Regarde si le nombre de tentes est bon
        test = test && nb_tents_row[i] == game_get_expected_nb_tents_row(g_new, i); // Regarde si le nombre de tentes est bon
    }

    game_delete(g_new);
    game_delete(g_ref);
    return test;
}

bool test_game_new_empty(void)
{
    game g = game_new_empty();
    bool test = true;
    test = test && g != NULL;

    for (uint i = 0; i < DEFAULT_SIZE; i++) // regarde si tout est bien EMPTY
    {
        for (uint j = 0; j < DEFAULT_SIZE; j++)
        {
            test = test && game_get_square(g, i, j) == EMPTY;
        }
    }

    for (uint i = 0; i < DEFAULT_SIZE; i++) // regarde si l'expect de tents est bien 0 partout
    {
        test = test && game_get_expected_nb_tents_row(g, i) == 0;
        test = test && game_get_expected_nb_tents_col(g, i) == 0;
    }

    test = test && game_get_current_nb_tents_all(g) == 0; // verification de fin (normalement c'est bon si ça passe la boucle de dessus)

    game_delete(g);
    return test;
}

bool test_game_copy(void)
{

    game g_orig = game_default();
    bool test = true;
    test = test && g_orig != NULL;

    game_play_move(g_orig, 0, 1, TENT);  // (0,1) est EMPTY
    game_play_move(g_orig, 1, 0, GRASS); // (1,0) est EMPTY

    game g_copy = game_copy(g_orig);
    test = test && g_copy != NULL;

    test = test && game_equal(g_orig, g_copy); // vérifié l'identicité

    game_play_move(g_orig, 1, 1, TENT); // (1,1) etait EMPTY
    test = test && game_get_square(g_orig, 1, 1) == TENT;
    test = test && game_get_square(g_copy, 1, 1) == EMPTY; // La copie doit rester EMPTY
    test = test && !game_equal(g_orig, g_copy);            // Ils sont maintenant différents

    game_play_move(g_copy, 2, 0, GRASS); // (2,0) etait EMPTY
    test = test && game_get_square(g_copy, 2, 0) == GRASS;
    test = test && game_get_square(g_orig, 2, 0) == EMPTY; // L'original doit rester EMPTY

    game_delete(g_orig);

    game g_fill = game_default();
    game_fill_grass(g_fill);
    g_copy = game_copy(g_fill);
    test = test && game_equal(g_fill, g_copy);

    game_delete(g_fill);

    game g_empty = game_new_empty();
    g_copy = game_copy(g_empty);
    test = test && game_equal(g_empty, g_copy);

    game_delete(g_copy);
    game_delete(g_empty);
    return test;
}

//  est ce qu'il faut faire les test pour les toutes expected colonnes ? ou seulement 2 conviennent parce que mashallah le temps que ça va prendre
bool test_game_equal(void)
{
    game g1 = game_default();
    game g2 = game_default();
    bool test = true;
    test = test && g1 != NULL && g2 != NULL;

    test = test && game_equal(g1, g2); // reagrde l'egalité simple

    test = test && game_get_current_nb_tents_all(g1) == game_get_current_nb_tents_all(g2);
    test = test && game_get_expected_nb_tents_all(g1) == game_get_expected_nb_tents_all(g2);

    game g_copy = game_copy(g1);
    test = test && g_copy != NULL;
    test = test && game_equal(g1, g_copy);
    game_delete(g_copy);

    game_play_move(g2, 0, 1, TENT); // (0,1) etait EMPTY par défaut
    test = test && !game_equal(g1, g2);

    game_play_move(g2, 0, 1, GRASS); // (0,1) etait TENT
    test = test && !game_equal(g1, g2);

    game g_fill = game_default();
    game_fill_grass(g_fill);
    game gf1 = game_copy(g_fill);

    game_play_move(g_fill, 0, 1, TENT); // (0,1) etait GRASS par défaut
    test = test && !game_equal(g_fill, gf1);

    game_play_move(g_fill, 0, 1, EMPTY); // (0,1) etait TENT
    test = test && !game_equal(g_fill, gf1);

    game_delete(g_fill);
    game_delete(gf1);
    game_delete(g2);
    g2 = game_default();

    game_set_expected_nb_tents_row(g2, 0, 99); // Valeur par défaut est 1
    test = test && !game_equal(g1, g2);

    game_delete(g2);
    g2 = game_default();

    game_set_expected_nb_tents_row(g2, 1, 99); // meme test mais pour row 1 au lieu de 0
    test = test && !game_equal(g1, g2);

    game_delete(g2);
    g2 = game_default();

    // Test 5: Différence dans nb_tents_col
    game_set_expected_nb_tents_col(g2, 0, 99); // Valeur par défaut est 3
    test = test && !game_equal(g1, g2);

    game_delete(g2);
    g2 = game_default();

    // Test 5: Différence dans nb_tents_col
    game_set_expected_nb_tents_col(g2, 1, 99); // meme test que precedemment
    test = test && !game_equal(g1, g2);
    test = test && game_get_current_nb_tents_col(g1, 1) != game_get_expected_nb_tents_col(g2, 1);

    game_delete(g1);
    game_delete(g2);
    return test;
}

// je sais pas trop si c'est ce qui est demandé genre parce que la ça va crash dans tt les cas... peut etre faire un $ echo?
bool test_game_delete(void)
{
    // on check juste l'absence de SegFault

    game g1 = game_default();
    bool test01 = g1 != NULL;
    game_delete(g1); // pas de crash

    game g2 = game_new_empty();
    bool test02 = g2 != NULL;
    game_delete(g2); // pas de crash

    game g_orig = game_default();
    game_play_move(g_orig, 0, 1, TENT);
    game g_copy = game_copy(g_orig);

    game_delete(g_orig); // pas de crash
    game_delete(g_copy); // pas de crash

    return test01 && test02; // Si on arrive ici, pas de crash mais bon ... ça me titille mashallah
}

bool test_game_play_move(void)
{
    // On ne teste que les coups valides pas les coups illegals mais jsp si faut tester quand meme
    // tent -> empty
    // tent -> tent
    // grass -> empty
    // empty -> tent
    // empty -> empty
    // tent -> grass
    // grass -> tent
    // empty -> grass
    // grass -> grass
    // 9 tests en tout

    game g = game_default();

    bool test = true;

    test = test && g != NULL;
    uint nb_tents_row[DEFAULT_SIZE] = {1, 2, 1, 2, 2, 0, 1, 3};
    uint nb_tents_col[DEFAULT_SIZE] = {3, 0, 2, 1, 2, 1, 2, 1};
    square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
        TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, TREE, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY,
        TREE, EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, TREE, TREE, EMPTY, EMPTY, TREE, TREE, EMPTY};

    game g_ref = game_new(squares, nb_tents_row, nb_tents_col);

    // on place une tent sur un empty
    uint row0_before = game_get_current_nb_tents_row(g, 0);
    uint col1_before = game_get_current_nb_tents_col(g, 1);
    uint all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 0, 1, TENT);
    test = test && game_get_square(g, 0, 1) == TENT; // on check si ça a bien marché et que c'est bien TENT
    test = test && game_get_current_nb_tents_row(g, 0) == row0_before + 1;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before + 1;
    test = test && game_get_current_nb_tents_all(g) == all_before + 1; // on regarde ce que ça implique
    squares[1] = TENT;
    g_ref = game_new(squares, nb_tents_row, nb_tents_col);
    test = test && game_equal(g, g_ref);

    // test tent sur tent
    row0_before = game_get_current_nb_tents_row(g, 0);
    col1_before = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 0, 1, TENT);
    test = test && game_get_square(g, 0, 1) == TENT; // on check si ça a bien marché et que c'est bien TENT
    test = test && game_get_current_nb_tents_row(g, 0) == row0_before;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before;
    test = test && game_get_current_nb_tents_all(g) == all_before; // on regarde ce que ça implique
    test = test && game_equal(g, g_ref);

    uint row1_before = game_get_current_nb_tents_row(g, 1);
    col1_before = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    // on place de l'herbe sur un empty
    game_play_move(g, 1, 1, GRASS);
    test = test && game_get_square(g, 1, 1) == GRASS; // on check si ça a bien marché et que c'est bien grass
    test = test && game_get_current_nb_tents_row(g, 1) == row1_before;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before;
    test = test && game_get_current_nb_tents_all(g) == all_before; // implication
    squares[9] = GRASS;
    g_ref = game_new(squares, nb_tents_row, nb_tents_col);
    test = test && game_equal(g, g_ref);

    // empty sur TENT
    row0_before = game_get_current_nb_tents_row(g, 0);
    col1_before = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 0, 1, EMPTY);

    test = test && game_get_square(g, 0, 1) == EMPTY; // on check si ça a bien marché et que c'est bien EMPTY
    test = test && game_get_current_nb_tents_row(g, 0) == row0_before - 1;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before - 1;
    test = test && game_get_current_nb_tents_all(g) == all_before - 1; // implication
    squares[1] = EMPTY;
    g_ref = game_new(squares, nb_tents_row, nb_tents_col);
    test = test && game_equal(g, g_ref);

    // empty sur empty
    row0_before = game_get_current_nb_tents_row(g, 0);
    col1_before = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 0, 1, EMPTY);

    test = test && game_get_square(g, 0, 1) == EMPTY; // on check si ça a bien marché et que c'est bien EMPTY
    test = test && game_get_current_nb_tents_row(g, 0) == row0_before;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before;
    test = test && game_get_current_nb_tents_all(g) == all_before; // implication

    test = test && game_equal(g, g_ref);

    // tent sur grass . On a bien grass en 1 1 grace au test 2 et si on est la ça veut dire que c'est success
    row1_before = game_get_current_nb_tents_row(g, 1);
    uint col1_before_2 = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 1, 1, TENT);
    test = test && game_get_square(g, 1, 1) == TENT;
    test = test && game_get_current_nb_tents_row(g, 1) == row1_before + 1;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before_2 + 1;
    test = test && game_get_current_nb_tents_all(g) == all_before + 1; // implication
    squares[9] = TENT;
    g_ref = game_new(squares, nb_tents_row, nb_tents_col);
    test = test && game_equal(g, g_ref);

    // grass sur tent
    row1_before = game_get_current_nb_tents_row(g, 1);
    col1_before_2 = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 1, 1, GRASS);
    test = test && game_get_square(g, 1, 1) == GRASS;
    test = test && game_get_current_nb_tents_row(g, 1) == row1_before - 1;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before_2 - 1;
    test = test && game_get_current_nb_tents_all(g) == all_before - 1; // ==>

    squares[9] = GRASS;
    g_ref = game_new(squares, nb_tents_row, nb_tents_col);
    test = test && game_equal(g, g_ref);

    // test grass sur grass
    row1_before = game_get_current_nb_tents_row(g, 1);
    col1_before_2 = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 1, 1, GRASS);
    test = test && game_get_square(g, 1, 1) == GRASS;
    test = test && game_get_current_nb_tents_row(g, 1) == row1_before;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before_2;
    test = test && game_get_current_nb_tents_all(g) == all_before; // ==>
    test = test && game_equal(g, g_ref);

    row1_before = game_get_current_nb_tents_row(g, 1);
    col1_before_2 = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    // grass sur grass
    game_play_move(g, 1, 1, EMPTY);
    test = test && game_get_square(g, 1, 1) == EMPTY;
    test = test && game_get_current_nb_tents_row(g, 1) == row1_before;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before_2;
    test = test && game_get_current_nb_tents_all(g) == all_before; // ==>

    squares[9] = EMPTY;
    g_ref = game_new(squares, nb_tents_row, nb_tents_col);
    test = test && game_equal(g, g_ref);

    game_delete(g_ref);
    game_delete(g); // fin mashallah
    return test;
}

bool test_game_check_move(void)
{
    game g = game_default();
    bool test = true;

    test = test && g != NULL;

    // illegal
    test = test && !game_check_move(g, 0, DEFAULT_SIZE + 200, TENT); // trop grand
    test = test && !game_check_move(g, DEFAULT_SIZE + 200, 0, TENT);
    test = test && !game_check_move(g, 0, DEFAULT_SIZE, TENT); // trop grand
    test = test && !game_check_move(g, DEFAULT_SIZE, 0, TENT);
    test = test && !game_check_move(g, -1, 0, TENT);
    test = test && !game_check_move(g, 0, -1, TENT);

    // placer un arbre
    test = test && !game_check_move(g, 0, 1, TREE); // Sur EMPTY
    test = test && !game_check_move(g, 0, 0, TREE); // Sur TREE
    game_play_move(g, 0, 1, GRASS);
    test = test && !game_check_move(g, 0, 1, TREE); // Sur GRASS

    // sur arbre
    test = test && !game_check_move(g, 0, 0, TENT);
    test = test && !game_check_move(g, 0, 0, GRASS);
    test = test && !game_check_move(g, 0, 0, EMPTY);

    // sur empty
    test = test && game_check_move(g, 0, 1, TENT);
    test = test && game_check_move(g, 0, 1, GRASS);
    test = test && game_check_move(g, 0, 1, EMPTY);

    game_play_move(g, 1, 0, TENT);

    // sur tent
    test = test && game_check_move(g, 1, 0, GRASS);
    test = test && game_check_move(g, 1, 0, EMPTY);
    test = test && game_check_move(g, 1, 0, TENT);

    game_play_move(g, 1, 1, GRASS);

    // sur grass
    test = test && game_check_move(g, 1, 1, TENT);
    test = test && game_check_move(g, 1, 1, EMPTY);
    test = test && game_check_move(g, 1, 1, GRASS);

    game_delete(g);
    return test;
}

bool test_game_get_current_nb_tents_all(void)
{

    // creer un jeu avec new ou default et je vais ensuite compare les nombres de colonnes avec ce que ça devrait renvoyer.
    game g = game_default();
    bool test = true;

    test = test && g != NULL;

    uint nb_tents_row[DEFAULT_SIZE] = {1, 2, 1, 2, 2, 0, 1, 3};
    uint nb_tents_col[DEFAULT_SIZE] = {3, 0, 2, 1, 2, 1, 2, 1};
    square squares[DEFAULT_SIZE * DEFAULT_SIZE] = {
        TREE, EMPTY, EMPTY, EMPTY, EMPTY, TREE, TREE, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TENT, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TREE, EMPTY,
        TREE, EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, TENT, TREE, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, TREE, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        EMPTY, TREE, TREE, EMPTY, EMPTY, TREE, TREE, EMPTY};

    game g_ref = game_new(squares, nb_tents_row, nb_tents_col);
    test = test && g_ref != NULL;
    test = test && game_get_current_nb_tents_all(g) == 0;
    test = test && game_get_current_nb_tents_all(g_ref) == 2;

    // je vais faire une boucle pour savoir le nombre de current par ligne et par colonne
    int sum_g = 0;
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        sum_g = sum_g + game_get_current_nb_tents_row(g, i);
    }
    int sum_g_ref = 0;
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        sum_g_ref = sum_g_ref + game_get_current_nb_tents_row(g_ref, i);
    }
    int sum_g_c = 0;
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        sum_g_c = sum_g_c + game_get_current_nb_tents_col(g, i);
    }
    int sum_g_ref_c = 0;
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        sum_g_ref_c = sum_g_ref_c + game_get_current_nb_tents_col(g_ref, i);
    }

    test = test && game_get_current_nb_tents_all(g) == sum_g;
    test = test && game_get_current_nb_tents_all(g_ref) == sum_g_ref;

    test = test && game_get_current_nb_tents_all(g) == sum_g_c;
    test = test && game_get_current_nb_tents_all(g_ref) == sum_g_ref_c;

    game_play_move(g, 1, 0, TENT);
    int val = game_get_current_nb_tents_all(g);
    test = test && game_get_current_nb_tents_all(g) == val + 1;
    game_play_move(g, 1, 0, EMPTY);

    game_play_move(g, 3, 1, TENT);
    val = game_get_current_nb_tents_all(g);
    test = test && game_get_current_nb_tents_all(g) == val + 1;
    game_play_move(g, 3, 1, EMPTY);

    game_play_move(g, 2, 2, TENT);
    val = game_get_current_nb_tents_all(g);
    test = test && game_get_current_nb_tents_all(g) == val + 1;
    game_play_move(g, 2, 2, EMPTY);

    game_play_move(g, 3, 3, TENT);
    val = game_get_current_nb_tents_all(g);
    test = test && game_get_current_nb_tents_all(g) == val + 1;
    game_play_move(g, 3, 3, EMPTY);

    game_play_move(g, 4, 3, TENT);
    val = game_get_current_nb_tents_all(g);
    test = test && game_get_current_nb_tents_all(g) == val + 1;
    game_play_move(g, 4, 3, EMPTY);

    game_play_move(g, 5, 1, TENT);
    val = game_get_current_nb_tents_all(g);
    test = test && game_get_current_nb_tents_all(g) == val + 1;
    game_play_move(g, 5, 1, EMPTY);

    game_play_move(g, 6, 1, TENT);
    val = game_get_current_nb_tents_all(g);
    test = test && game_get_current_nb_tents_all(g) == val + 1;
    game_play_move(g, 6, 1, EMPTY);

    game_play_move(g, 7, 0, TENT);
    val = game_get_current_nb_tents_all(g);
    test = test && game_get_current_nb_tents_all(g) == val + 1;
    game_play_move(g, 7, 0, EMPTY);

    g_ref = game_default_solution();
    test = test && game_get_current_nb_tents_all(g_ref) == 12;

    game_delete(g);
    game_delete(g_ref);
    return test;
}

/*bool test_game_get_current_nb_tents_col(void)
{
}

bool test_game_get_current_nb_tents_row(void)
{
}*/