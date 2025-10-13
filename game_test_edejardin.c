#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_dummy()
{
    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (strcmp("dummy", argv[1]) == 0)
    {
        return test_dummy();
    }
    return EXIT_FAILURE;
}
