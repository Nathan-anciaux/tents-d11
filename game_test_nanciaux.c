#include <stdio.h>
#include <stdlib.h>
#include <string.h>



int test_dummy() {
    return EXIT_SUCCESS;
}

void usage( int argc, char *argv[]) {
    fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        usage(argc, argv);
    }
    if (strcmp(argv[1], "dummy") == 0) {
        return test_dummy();
    }
    return EXIT_FAILURE;
}