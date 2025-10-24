#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"

bool test_game_set_square();

bool test_game_set_expected_nb_tents_row();

bool test_game_set_expected_nb_tents_col();

bool test_game_get_square();

bool test_game_get_expected_nb_tents_row();

bool test_game_get_expected_nb_tents_col();

bool test_game_get_expected_nb_tents_all();

bool test_game_get_current_nb_tents_row();

bool test_game_get_current_nb_tents_col();

bool test_game_get_current_nb_tents_all();

bool test_game_get_ajacent_square();

int test_dummy()
{
    return EXIT_SUCCESS;
}

bool test_game_set_square()
{
    game g = game_new_empty();
    if (g == NULL)
    {
        return false;
    }
    int L[] = {TENT, GRASS, EMPTY, TREE};
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                game_set_square(g, i, j, L[k]);
                if (game_get_square(g, i, j) != L[k])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool test_game_set_expected_nb_tents_row()
{
    game g = game_new_empty();
    if (g == NULL)
    {
        return false;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            game_set_expected_nb_tents_row(g, i, j);
            if (game_get_expected_nb_tents_row(g, i) != j)
            {
                return false;
            }
        }
    }
    return true;
}

bool test_game_set_expected_nb_tents_col()
{
    game g = game_new_empty();
    if (g == NULL)
    {
        return false;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            game_set_expected_nb_tents_col(g, i, j);
            if (game_get_expected_nb_tents_col(g, i) != j)
            {
                return false;
            }
        }
    }
    return true;
}

bool test_game_get_square()
{
    game g = game_new_empty();
    int L[] = {TENT, GRASS, EMPTY, TREE};
    if (g == NULL)
    {
        return false;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            for (int k = 0; k < 4; k++)
            {
                game_set_square(g, i, j, L[k]);
                if (game_get_square(g, i, j) != L[k])
                {
                    return false;
                }
            }
        }
    }
    return true;
}

bool test_game_get_expected_nb_tents_row()
{
    game g = game_new_empty();
    if (g == NULL)
    {
        return false;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            game_set_expected_nb_tents_row(g, i, j);
            if (game_get_expected_nb_tents_row(g, i) != j)
            {
                return false;
            }
        }
    }
    return true;
}

bool test_game_get_expected_nb_tents_col()
{
    game g = game_new_empty();
    if (g == NULL)
    {
        return false;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            game_set_expected_nb_tents_col(g, i, j);
            if (game_get_expected_nb_tents_col(g, i) != j)
            {
                return false;
            }
        }
    }
    return true;
}

bool test_game_get_expected_nb_tents_all()
{
    game g = game_new_empty();
    int s = 0;
    if (g == NULL)
    {
        return false;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        s = s + game_get_expected_nb_tents_row(g, i);
        if (game_get_expected_nb_tents_all(g) != s)
        {
            return false;
        }
    }
    return true;
}

bool test_game_get_current_nb_tents_row()
{
    game g = game_new_empty();
    int s = 0;
    if (g == NULL)
    {
        return false;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            if (game_get_square(g, i, j) == TENT)
            {
                s++;
            }
            if (game_get_current_nb_tents_row(g, i) != s)
            {
                return false;
            }
            s = 0;
        }
    }
    return true;
}

bool test_game_get_current_nb_tents_col()
{
    game g = game_new_empty();
    int s = 0;
    if (g == NULL)
    {
        return false;
    }
    for (int j = 0; j < DEFAULT_SIZE; j++)
    {
        for (int i = 0; i < DEFAULT_SIZE; i++)
        {
            if (game_get_square(g, i, j) == TENT)
            {
                s++;
            }
            if (game_get_current_nb_tents_col(g, j) != s)
            {
                return false;
            }
            s = 0;
        }
    }
    return true;
}
bool test_game_get_current_nb_tents_all()
{
    game g = game_new_empty();
    int s = 0;
    if (g == NULL)
    {
        return false;
    }
    for (int i = 0; i < DEFAULT_SIZE; i++)
    {
        for (int j = 0; j < DEFAULT_SIZE; j++)
        {
            if (game_get_square(g, i, j) == TENT)
            {
                s++;
            }
        }
    }
    if (game_get_current_nb_tents_all(g) != s)
    {
        return false;
    }
    return true;
}

int main(int argc, char *argv[])
{
    bool ok = 0;
    if (argc == 1)
    {
        fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    if (strcmp("dummy", argv[1]) == 0)
    {
        ok = test_dummy();
    }
    if (strcmp("game_set_square",argv[1] ) == 0)
    {
        ok = test_game_set_square();
    }
    if (strcmp("game_set_expected_nb_tents_row", argv[1]) == 0)
    {
        ok = test_game_set_expected_nb_tents_row();
    }
    if (strcmp("game_set_expected_nb_tents_col", argv[1]) == 0)
    {
        ok = test_game_set_expected_nb_tents_col();
    }
    if (strcmp("game_get_square", argv[1]) == 0)
    {
        ok = test_game_get_square();
    }
    if (strcmp("game_get_expected_nb_tents_row",argv[1]) == 0)
    {
        ok = test_game_get_expected_nb_tents_row();
    }
    if (strcmp("game_get_expected_nb_tents_col" , argv[1]) == 0)
    {
        ok = test_game_get_expected_nb_tents_col();
    }
    if (strcmp("game_get_expected_nb_tents_all",argv[1] ) == 0)
    {
        ok = test_game_get_expected_nb_tents_all();
    }
    if (strcmp("game_get_current_nb_tents_row", argv[1]) == 0)
    {
        ok = test_game_get_current_nb_tents_row();
    }
    if (strcmp("game_get_current_nb_tents_col", argv[1]) == 0)
    {
        ok = test_game_get_current_nb_tents_col();
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