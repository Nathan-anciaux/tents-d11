#include "game_tools.h"

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game_aux.h"
#include "game.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_struct.h"

game game_load(char* filename);

void game_save(cgame g, char* filename);

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_trees, uint nb_bigtrees, bool solution);