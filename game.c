// game.c
#include "game.h"

#include <stdio.h>
#include <stdlib.h>

#include "game_struct.h"

game game_new(square* squares, uint* nb_tents_row, uint* nb_tents_col)
// Creates a new game with default size and initializes it
{
  // Alocation de la mémoire pour tous les attributs de g
  game g = malloc(sizeof(struct game_s));

  if (g == NULL) {
    free(g);
    fprintf(stderr, "ERROR");
    exit(EXIT_FAILURE);
  }

  g->tab = malloc(DEFAULT_SIZE * DEFAULT_SIZE * sizeof(square));
  g->nb_tent_col_expected = malloc(DEFAULT_SIZE * sizeof(uint));
  g->nb_tent_row_expected = malloc(DEFAULT_SIZE * sizeof(uint));

  if (g->tab == NULL || g->nb_tent_col_expected == NULL || g->nb_tent_row_expected == NULL) {
    free(g->tab);
    free(g->nb_tent_col_expected);
    free(g->nb_tent_row_expected);

    fprintf(stderr, "ERROR");
    exit(EXIT_FAILURE);
  }

  // Remplissage des tableaux avec toutes les valeurs attendues
  for (uint i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    g->tab[i] = squares[i];
  }

  for (uint i = 0; i < DEFAULT_SIZE; i++) {
    g->nb_tent_col_expected[i] = nb_tents_col[i];
    g->nb_tent_row_expected[i] = nb_tents_row[i];
  }

  return g;
}

game game_new_empty(void)
// Creates a new empty game with defaut size
{
  // Allocation des éléments à remplir
  square* tab = malloc(sizeof(square) * DEFAULT_SIZE * DEFAULT_SIZE);
  uint* expected_tent_row = malloc(sizeof(uint) * DEFAULT_SIZE);
  uint* expected_tent_col = malloc(sizeof(uint) * DEFAULT_SIZE);

  // Remplissage de ces éléments pour qu'ils soient considérés comme vide (EMPTY et 0)
  for (int i = 0; i < DEFAULT_SIZE * DEFAULT_SIZE; i++) {
    tab[i] = EMPTY;
  }

  for (int i = 0; i < DEFAULT_SIZE; i++) {
    expected_tent_col[i] = 0;
    expected_tent_row[i] = 0;
  }

  game g = game_new(tab, expected_tent_row, expected_tent_col);
  return g;
}

game game_copy(cgame g)
// 	Duplicates a game
{
  // Récupère les attributs de g et créer une nouvelle partie g2 avec ceux-ci
  game g2 = game_new(g->tab, g->nb_tent_row_expected, g->nb_tent_col_expected);
  return g2;
}

bool game_equal(cgame g1, cgame g2)
// Tests if two games are equal
{
  return 1;
}

void game_delete(game g)
// Deletes the game and frees the allocated memory
{
  return;
}

void game_set_square(game g, uint i, uint j, square s)
// Sets the value of a given square
{
  g->tab[i * DEFAULT_SIZE + j] = s;
  return;
}

square game_get_square(cgame g, uint i, uint j)
// Gets the value of a given square
{
  return g->tab[i * DEFAULT_SIZE + j];
}

void game_set_expected_nb_tents_row(game g, uint i, uint nb_tents)
// Sets the expected number of tents in a given row.
{
  return;
}

void game_set_expected_nb_tents_col(game g, uint j, uint nb_tents)
// Sets the expected number of tents in a given column.
{
  return;
}

uint game_get_expected_nb_tents_row(cgame g, uint i)
// Gets the expected number of tents in a given row
{
  uint u = 0;
  return u;
}

uint game_get_expected_nb_tents_col(cgame g, uint j)
// Gets the expected number of tents in a given column
{
  uint u = 0;
  return u;
}

uint game_get_expected_nb_tents_all(cgame g)
// Gets the expected number of tents on the whole grid.
{
  uint u = 0;
  return u;
}

uint game_get_current_nb_tents_row(cgame g, uint i)
// Gets the current number of tents in a given row.
{
  uint u = 0;
  return u;
}

uint game_get_current_nb_tents_col(cgame g, uint j)
// Gets the current number of tents in a given column.
{
  uint u = 0;
  return u;
}

uint game_get_current_nb_tents_all(cgame g)
// Gets the current number of tents on the whole grid.
{
  uint u = 0;
  return u;
}

void game_play_move(game g, uint i, uint j, square s)
// Plays a move in a given square
{
  return;
}

bool game_check_move(cgame g, uint i, uint j, square s)
// Checks if a given move in a square is legal
{
  return 1;
}

bool game_won(cgame g)
// Checks if the game is won
{
  return 1;
}

void game_fill_grass(game g)
// Fills the board with grass
{
  //Remplace tous les carrés qui sont empty par de l'herbe (Ici on fait le choix de faire 2 boucles imbriquées au lieu d'une afin d'utiliser des fonctions déjà faite)
  for (int i=0; i<DEFAULT_SIZE; i++){
    for (int j=0; j<DEFAULT_SIZE; j++){
      if (game_get_square(g,i,j) == EMPTY){
        game_set_square(g,i,j,GRASS);
      }
    }
  }
  return;
}

void game_restart(game g)
// Restarts a game.
{
  // On remplace tous les square par empty si ce n'est pas des arbres
  for (int i=0; i<DEFAULT_SIZE; i++){
    for (int j=0; j<DEFAULT_SIZE; j++){
      if (!(game_get_square(g,i,j) == TREE)){
        game_set_square(g,i,j,EMPTY);
      }
    }
  }
  return;
}