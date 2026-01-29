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

static char square_to_char(square s){
    char c;
    switch (s){
        case EMPTY:
            c = 'E';
            return c;
        case TREE:
            c = 'X';
            return c;
        case GRASS:
            c = 'G';
            return c;
        case BIGTREE:
            c = 'B';
            return c;
        case TENT:
            c = 'T';
            return c;
        default:
            return 'E';
    }
}

static char char_to_square(char c){
    square s;
    switch (c){
        case 'E':
            s = EMPTY;
            return s;
        case 'G':
            s = GRASS;
            return s;
        case 'T':
            s = TENT;
            return s;
        case 'B':
            s = BIGTREE;
            return s;
        case 'X':
            s = TREE;
            return s;
        default:
            return EMPTY;
    }
}

game game_load(char* filename){
    FILE *file = fopen(filename, "r");
    if (file ==NULL){
        fprintf(stderr, "Fichier inexistant %s\n", filename);
        return NULL;
    }

    // we check the row and col and wrapp
    uint row, col, wrap;
    fscanf(file, "%u %u %u\n", &row, &col, &wrap);
    
    uint nb_row[row];
    uint nb_col[col];
    uint tmp;

    for (uint i=0; i< row; i++){
        fscanf(file, "%u ", &tmp);
        nb_row[i] = tmp;
    }
    fscanf(file, "\n");

    for (uint i=0; i< col; i++){
        fscanf(file, "%u ", &tmp);
        nb_col[i] = tmp;
    }
    fscanf(file, "\n");
    char char_tmp;
    
    // every square in a tab
    square squares[col * row];
    for (uint r =0; r < row; r++){
        for (uint c =0; c < col; c++){
            if (fscanf(file, " %c", &char_tmp) == 1){
                squares[r * col + c] = char_to_square(char_tmp);
            }
        }
        fscanf(file, "\n");
    }

    fclose(file);
    // creating the game
    game g = game_new_ext(row, col, squares, nb_row, nb_col, wrap);
    return g;

}

void game_save(cgame g, char* filename){
    FILE *file = fopen(filename, "w");
    if (file == NULL)
        return; // la sauvegarde a échoué

    // champs de game
    fprintf(file, "%u %u %u\n", g->nb_rows, g->nb_cols, g->wrapping);

    // row and col
    for (uint i=0; i<g->nb_rows; i++){
        fprintf(file, "%u ", game_get_expected_nb_tents_row(g, i));
    }
    fprintf(file, "\n");

    for (uint i=0; i<g->nb_cols; i++){
        fprintf(file, "%u ", game_get_expected_nb_tents_col(g, i));
    }
    fprintf(file, "\n");

    // copy of every game's square with a space between each other
    for (uint row=0; row < g->nb_rows; row++){
        for (uint col=0; col < g->nb_cols; col++){
            char c = game_get_square(g, row, col);
            fprintf(file, "%c ", square_to_char(c));
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

game game_random(uint nb_rows, uint nb_cols, bool wrapping, uint nb_trees, uint nb_bigtrees, bool solution);