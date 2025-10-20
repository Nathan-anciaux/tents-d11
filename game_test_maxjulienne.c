#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "game.h"
#include "game_aux.h"


int test_game_won()
{
    game g = game_default();
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