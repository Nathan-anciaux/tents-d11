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

// le main permet denvoyer les tests de toutes les fonctions
int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s <test_name>\n", argv[0]);
        fprintf(stderr, "Available tests:\n");
        fprintf(stderr, "  game_new\n");
        fprintf(stderr, "  game_new_empty\n");
        fprintf(stderr, "  game_copy\n");
        fprintf(stderr, "  game_equal\n");
        fprintf(stderr, "  game_delete\n");
        fprintf(stderr, "  game_play_move\n");
        fprintf(stderr, "  game_check_move\n");
        return EXIT_FAILURE;
    }

    typedef struct
    {
        const char *name;
        bool (*test_func)(void);
    } test_entry;

    test_entry tests[] = {
        {"game_new", test_game_new},
        {"game_new_empty", test_game_new_empty},
        {"game_copy", test_game_copy},
        {"game_equal", test_game_equal},
        {"game_delete", test_game_delete},
        {"game_play_move", test_game_play_move},
        {"game_check_move", test_game_check_move}};

    bool ok = false;
    const char *test_name = argv[1];
    bool test_found = false;

    for (size_t i = 0; i < sizeof(tests) / sizeof(tests[0]); ++i)
    {
        if (strcmp(tests[i].name, test_name) == 0)
        {
            ok = tests[i].test_func();
            test_found = true;
            break;
        }
    }

    if (!test_found)
    {
        fprintf(stderr, "Unknown test: %s\n", test_name);
        return EXIT_FAILURE;
    }

    if (ok)
    {
        fprintf(stdout, "Test \"%s\" finished: SUCCESS\n", test_name);
        return EXIT_SUCCESS;
    }
    else
    {
        fprintf(stderr, "Test \"%s\" finished: FAILURE\n", test_name);
        return EXIT_FAILURE;
    }
}
/*
int test_dummy()
{
    return EXIT_SUCCESS;
} */

bool test_game_new(void)
{
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

    test = test && game_equal(g_new, g_ref); // source d'erreur mais jsp pourquoi

    // Le jeu g_new ne doit PAS changer (il doit avoir fait une copie).
    squares[0] = EMPTY;
    nb_tents_row[0] = 99;
    nb_tents_col[0] = 99;

    test = test && game_get_square(g_new, 0, 0) == TREE;          // Doit être resté TREE
    test = test && game_get_expected_nb_tents_row(g_new, 0) == 1; // Doit être resté 1
    test = test && game_get_expected_nb_tents_col(g_new, 0) == 3; // Doit être resté 3

    squares[0] = TREE;
    nb_tents_row[0] = 1;
    nb_tents_col[0] = 3;

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

// il faut faire attention avec default size !! parce qu'on peut la changer mais jsp encore ce que c'est la variable pas par default
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
    game_delete(g_copy);
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

    game g_copy = game_copy(g1);
    test = test && g_copy != NULL;
    test = test && game_equal(g1, g_copy);
    game_delete(g_copy);

    game_play_move(g2, 0, 1, TENT); // (0,1) etait EMPTY par défaut
    test = test && !game_equal(g1, g2);

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

    // on place une tent sur un empty
    uint row0_before = game_get_current_nb_tents_row(g, 0);
    uint col1_before = game_get_current_nb_tents_col(g, 1);
    uint all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 0, 1, TENT);
    test = test && game_get_square(g, 0, 1) == TENT; // on check si ça a bien marché et que c'est bien TENT
    test = test && game_get_current_nb_tents_row(g, 0) == row0_before + 1;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before + 1;
    test = test && game_get_current_nb_tents_all(g) == all_before + 1; // on regarde ce que ça implique

    // test tent sur tent
    row0_before = game_get_current_nb_tents_row(g, 0);
    col1_before = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 0, 1, TENT);
    test = test && game_get_square(g, 0, 1) == TENT; // on check si ça a bien marché et que c'est bien TENT
    test = test && game_get_current_nb_tents_row(g, 0) == row0_before;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before;
    test = test && game_get_current_nb_tents_all(g) == all_before; // on regarde ce que ça implique

    // on place de l'herbe sur un empty
    game_play_move(g, 1, 1, GRASS);
    test = test && game_get_square(g, 1, 1) == GRASS; // on check si ça a bien marché et que c'est bien grass

    // empty sur TENT
    row0_before = game_get_current_nb_tents_row(g, 0);
    col1_before = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 0, 1, EMPTY);

    test = test && game_get_square(g, 0, 1) == EMPTY; // on check si ça a bien marché et que c'est bien EMPTY
    test = test && game_get_current_nb_tents_row(g, 0) == row0_before - 1;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before - 1;
    test = test && game_get_current_nb_tents_all(g) == all_before - 1; // implication

    // empty sur empty
    row0_before = game_get_current_nb_tents_row(g, 0);
    col1_before = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 0, 1, EMPTY);

    test = test && game_get_square(g, 0, 1) == EMPTY; // on check si ça a bien marché et que c'est bien EMPTY
    test = test && game_get_current_nb_tents_row(g, 0) == row0_before;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before;
    test = test && game_get_current_nb_tents_all(g) == all_before; // implication

    // tent sur grass . On a bien grass en 1 1 grace au test 2 et si on est la ça veut dire que c'est success
    uint row1_before = game_get_current_nb_tents_row(g, 1);
    uint col1_before_2 = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 1, 1, TENT);
    test = test && game_get_square(g, 1, 1) == TENT;
    test = test && game_get_current_nb_tents_row(g, 1) == row1_before + 1;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before_2 + 1;
    test = test && game_get_current_nb_tents_all(g) == all_before + 1; // implication

    // grass sur tent
    row1_before = game_get_current_nb_tents_row(g, 1);
    col1_before_2 = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 1, 1, GRASS);
    test = test && game_get_square(g, 1, 1) == GRASS;
    test = test && game_get_current_nb_tents_row(g, 1) == row1_before - 1;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before_2 - 1;
    test = test && game_get_current_nb_tents_all(g) == all_before - 1; // ==>

    // test grass sur grass
    row1_before = game_get_current_nb_tents_row(g, 1);
    col1_before_2 = game_get_current_nb_tents_col(g, 1);
    all_before = game_get_current_nb_tents_all(g);

    game_play_move(g, 1, 1, GRASS);
    test = test && game_get_square(g, 1, 1) == GRASS;
    test = test && game_get_current_nb_tents_row(g, 1) == row1_before;
    test = test && game_get_current_nb_tents_col(g, 1) == col1_before_2;
    test = test && game_get_current_nb_tents_all(g) == all_before; // ==>

    game_delete(g); // fin mashallah
    return test;
}

bool test_game_check_move(void)
{
    game g = game_default();
    bool test = true;

    test = test && g != NULL;

    // illegal
    test = test && !game_check_move(g, 0, DEFAULT_SIZE, TENT); // trop grand
    test = test && !game_check_move(g, DEFAULT_SIZE, 0, TENT);
    test = test && !game_check_move(g, -1, 0, TENT);
    test = test && !game_check_move(g, 0, -1, TENT);

    // placer un arbre
    test = test && !game_check_move(g, 0, 1, TREE); // Sur EMPTY
    test = test && !game_check_move(g, 0, 0, TREE); // Sur TREE

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