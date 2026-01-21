#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "aux.h"
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"

bool test_dummy(void) { return true; }

//==========> SET SQUARE <==========//

bool test_set_square(void) {
  game g = game_new_empty();
  bool ok = true;

  // CAS TREE
  game_set_square(g, 0, 0, TREE);
  if (game_get_square(g, 0, 0) != TREE) ok = false;

  // CAS TENT
  game_set_square(g, 0, 0, TENT);
  if (game_get_square(g, 0, 0) != TENT) ok = false;

  // CAS GRASS
  game_set_square(g, 7, 7, GRASS);
  if (game_get_square(g, 7, 7) != GRASS) ok = false;

  // CAS EMPTY
  game_set_square(g, 0, 0, EMPTY);
  if (game_get_square(g, 0, 0) != EMPTY) ok = false;

  game_delete(g);

  game g2 = game_new_empty_ext(6, 8, false);

  // CAS TREE
  game_set_square(g2, 0, 0, TREE);
  if (game_get_square(g2, 0, 0) != TREE) ok = false;

  // CAS TENT
  game_set_square(g2, 0, 0, TENT);
  if (game_get_square(g2, 0, 0) != TENT) ok = false;

  // CAS GRASS
  game_set_square(g2, 5, 7, GRASS);
  if (game_get_square(g2, 5, 7) != GRASS) ok = false;
  // CAS EMPTY
  game_set_square(g2, 0, 0, EMPTY);
  if (game_get_square(g2, 0, 0) != EMPTY) ok = false;

  game_delete(g2);

  return ok;
}

//==========> GET SQUARE <==========//

bool test_get_square(void) {
  game g = game_new_empty();
  bool ok = true;

  // CAS EMPTY
  if (game_get_square(g, 7, 7) != EMPTY) ok = false;

  // CAS TREE
  game_set_square(g, 0, 0, TREE);
  if (game_get_square(g, 0, 0) != TREE) ok = false;

  // CAS TENT
  game_set_square(g, 0, 0, TENT);
  if (game_get_square(g, 0, 0) != TENT) ok = false;

  // CAS GRASS
  game_set_square(g, 0, 0, GRASS);
  if (game_get_square(g, 0, 0) != GRASS) ok = false;

  game_delete(g);
  game g2 = game_new_empty_ext(6, 8, true);

  // CAS EMPTY
  if (game_get_square(g2, 5, 7) != EMPTY) ok = false;

  // CAS TREE
  game_set_square(g2, 0, 0, TREE);
  if (game_get_square(g2, 0, 0) != TREE) ok = false;
  // CAS TENT
  game_set_square(g2, 0, 0, TENT);
  if (game_get_square(g2, 0, 0) != TENT) ok = false;

  // CAS GRASS
  game_set_square(g2, 0, 0, GRASS);
  if (game_get_square(g2, 0, 0) != GRASS) ok = false;
  game_delete(g2);

  return ok;
}

//==========> GET CURRENT NB TENTS ROW <==========//

bool test_get_current_nb_tents_row(void) {
  game g = game_new_empty();
  bool ok = true;
  if (game_get_current_nb_tents_row(g, 0) != 0) ok = false;

  game_set_square(g, 0, 0, GRASS);
  if (game_get_current_nb_tents_row(g, 0) != 0) ok = false;

  game_set_square(g, 0, 0, TREE);
  if (game_get_current_nb_tents_row(g, 0) != 0) ok = false;

  game_set_square(g, 0, 0, TENT);
  if (game_get_current_nb_tents_row(g, 0) != 1) ok = false;

  game_set_square(g, 0, 0, EMPTY);
  if (game_get_current_nb_tents_row(g, 0) != 0) ok = false;

  game_set_square(g, 0, 7 - 1, TENT);
  if (game_get_current_nb_tents_row(g, 0) != 1) ok = false;

  for (int j = 0; j < 7; j++) {
    game_set_square(g, 7, j, TENT);
  }
  if (game_get_current_nb_tents_row(g, 7) != 7) ok = false;

  game_delete(g);

  game g2 = game_new_empty_ext(6, 8, true);
  if (game_get_current_nb_tents_row(g2, 0) != 0) ok = false;

  game_set_square(g2, 0, 0, GRASS);
  if (game_get_current_nb_tents_row(g2, 0) != 0) ok = false;
  game_set_square(g2, 0, 0, TREE);
  if (game_get_current_nb_tents_row(g2, 0) != 0) ok = false;

  game_set_square(g2, 0, 0, TENT);
  if (game_get_current_nb_tents_row(g2, 0) != 1) ok = false;

  game_set_square(g2, 0, 0, EMPTY);
  if (game_get_current_nb_tents_row(g2, 0) != 0) ok = false;

  game_set_square(g2, 0, 7, TENT);
  if (game_get_current_nb_tents_row(g2, 0) != 1) ok = false;

  for (int j = 0; j < 8; j++) {
    game_set_square(g2, 5, j, TENT);
  }
  if (game_get_current_nb_tents_row(g2, 5) != 8) ok = false;

  game_delete(g2);
  return ok;
}

//==========> GET CURRENT NB TENTS COL <==========//

bool test_get_current_nb_tents_col(void) {
  game g = game_new_empty();
  bool ok = true;

  if (game_get_current_nb_tents_col(g, 0) != 0) ok = false;

  game_set_square(g, 0, 0, GRASS);
  if (game_get_current_nb_tents_col(g, 0) != 0) ok = false;

  game_set_square(g, 0, 0, TREE);
  if (game_get_current_nb_tents_col(g, 0) != 0) ok = false;

  game_set_square(g, 0, 0, TENT);
  if (game_get_current_nb_tents_col(g, 0) != 1) ok = false;

  game_set_square(g, 0, 0, EMPTY);
  if (game_get_current_nb_tents_col(g, 0) != 0) ok = false;

  game_set_square(g, 5, 0, TENT);
  if (game_get_current_nb_tents_col(g, 0) != 1) ok = false;

  for (int i = 0; i < 6; i++) {  // Tente sur la dernière colonne
    game_set_square(g, i, 8 - 1, TENT);
  }
  if (game_get_current_nb_tents_col(g, 8 - 1) != 6) ok = false;

  game_delete(g);
  game g2 = game_new_empty_ext(6, 8, true);
  if (game_get_current_nb_tents_col(g2, 0) != 0) ok = false;

  game_set_square(g2, 0, 0, GRASS);
  if (game_get_current_nb_tents_col(g2, 0) != 0) ok = false;

  game_set_square(g2, 0, 0, TREE);
  if (game_get_current_nb_tents_col(g2, 0) != 0) ok = false;

  game_set_square(g2, 0, 0, TENT);
  if (game_get_current_nb_tents_col(g2, 0) != 1) ok = false;
  game_set_square(g2, 0, 0, EMPTY);
  if (game_get_current_nb_tents_col(g2, 0) != 0) ok = false;

  game_set_square(g2, 5, 0, TENT);
  if (game_get_current_nb_tents_col(g2, 0) != 1) ok = false;

  for (int i = 0; i < 6; i++) {
    game_set_square(g2, i, 7, TENT);
  }
  if (game_get_current_nb_tents_col(g2, 7) != 6) ok = false;

  game_delete(g2);

  return ok;
}

//==========> GET CURRENT NB TENTS ALL <==========//

bool test_get_current_nb_tents_all(void) {
  game g = game_new_empty();
  bool ok = true;

  if (game_get_current_nb_tents_all(g) != 0) ok = false;

  game_set_square(g, 0, 0, GRASS);
  if (game_get_current_nb_tents_all(g) != 0) ok = false;

  game_set_square(g, 0, 0, TREE);
  if (game_get_current_nb_tents_all(g) != 0) ok = false;

  game_set_square(g, 0, 0, TENT);
  if (game_get_current_nb_tents_all(g) != 1) ok = false;

  game_set_square(g, 0, 0, EMPTY);
  if (game_get_current_nb_tents_all(g) != 0) ok = false;

  game_set_square(g, 7, 0, TENT);
  game_set_square(g, 0, 8 - 1, TENT);
  game_set_square(g, 7, 8 - 1, TENT);
  if (game_get_current_nb_tents_all(g) != 3) ok = false;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      game_set_square(g, i, j, TENT);
    }
  }
  if (game_get_current_nb_tents_all(g) != 8 * 8) ok = false;

  game_delete(g);
  game g2 = game_new_empty_ext(6, 8, true);
  if (game_get_current_nb_tents_all(g2) != 0) ok = false;

  game_set_square(g2, 0, 0, GRASS);
  if (game_get_current_nb_tents_all(g2) != 0) ok = false;

  game_set_square(g2, 0, 0, TREE);
  if (game_get_current_nb_tents_all(g2) != 0) ok = false;

  game_set_square(g2, 0, 0, TENT);
  if (game_get_current_nb_tents_all(g2) != 1) ok = false;
  game_set_square(g2, 0, 0, EMPTY);
  if (game_get_current_nb_tents_all(g2) != 0) ok = false;

  game_set_square(g2, 5, 0, TENT);
  game_set_square(g2, 0, 7, TENT);
  game_set_square(g2, 5, 7, TENT);
  if (game_get_current_nb_tents_all(g2) != 3) ok = false;

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      game_set_square(g2, i, j, TENT);
    }
  }
  if (game_get_current_nb_tents_all(g2) != 6 * 8) ok = false;
  game_delete(g2);
  return ok;
}

//==========> CHECK MOVE <==========//

bool test_check_move(void) {
  game g = game_new_empty();
  bool ok = true;

  if (!game_check_move(g, 0, 0, EMPTY)) ok = false;
  if (!game_check_move(g, 0, 0, GRASS)) ok = false;
  if (!game_check_move(g, 0, 0, TENT)) ok = false;
  if (game_check_move(g, 0, 0, TREE)) ok = false;

  game_set_square(g, 0, 0, TREE);
  if (game_check_move(g, 0, 0, TENT)) ok = false;
  if (game_check_move(g, 0, 0, EMPTY)) ok = false;
  if (game_check_move(g, 0, 0, GRASS)) ok = false;
  if (game_check_move(g, 8, 0, TENT)) ok = false;
  if (game_check_move(g, 0, 8, GRASS)) ok = false;
  if (game_check_move(g, 8, 8, EMPTY)) ok = false;
  if (game_check_move(g, 8, 8, TREE)) ok = false;

  game_delete(g);
  return ok;
}

//==========> PLAY MOVE <==========//

bool test_play_move(void) {
  game g = game_new_empty();
  bool ok = true;

  game_play_move(g, 0, 0, TENT);
  if (game_get_square(g, 0, 0) != TENT) ok = false;

  game_play_move(g, 0, 0, EMPTY);
  if (game_get_square(g, 0, 0) != EMPTY) ok = false;

  game_play_move(g, 0, 0, GRASS);
  if (game_get_square(g, 0, 0) != GRASS) ok = false;

  game_play_move(g, 7, 7, GRASS);
  if (game_get_square(g, 7, 7) != GRASS) ok = false;

  game_play_move(g, 7, 7, TENT);
  if (game_get_square(g, 7, 7) != TENT) ok = false;

  game_delete(g);
  return ok;
}

//==========> WON <==========//

bool test_won(void) {
  bool ok = true;

  game g = game_new_empty();

  if (!game_won(g)) ok = false;
  game_delete(g);

  // Regle 1 Tentes adjacentes

  // 🟩🌲⛺⛺🌲🟩

  game g1 = game_new_empty();
  game_set_expected_nb_tents_row(g1, 0, 2);
  game_set_expected_nb_tents_col(g1, 2, 1);
  game_set_expected_nb_tents_col(g1, 3, 1);
  game_set_square(g1, 0, 1, TREE);
  game_set_square(g1, 0, 4, TREE);
  game_set_square(g1, 0, 2, TENT);
  game_set_square(g1, 0, 3, TENT);

  if (game_won(g1)) ok = false;
  game_delete(g1);

  // Regle 2 Compte tentes lignes / colonnes

  // ⛺🌲🟩🟩 1
  // 🟩🟩🌲⛺ 1
  // 1 0 0 2

  game g2 = game_new_empty();
  game_set_expected_nb_tents_row(g2, 0, 1);
  game_set_expected_nb_tents_row(g2, 1, 1);
  game_set_expected_nb_tents_col(g2, 0, 1);
  game_set_expected_nb_tents_col(g2, 3, 2);

  game_set_square(g2, 0, 1, TREE);
  game_set_square(g2, 1, 2, TREE);
  game_set_square(g2, 0, 0, TENT);
  game_set_square(g2, 0, 2, TENT);

  if (game_won(g2)) ok = false;
  game_delete(g2);

  // Regle 3 nb tentes = nb arbres

  // ⛺🌲⛺

  game g3 = game_new_empty();
  game_set_expected_nb_tents_row(g3, 0, 2);
  game_set_expected_nb_tents_col(g3, 0, 1);
  game_set_expected_nb_tents_col(g3, 2, 1);
  game_set_square(g3, 0, 1, TREE);
  game_set_square(g3, 0, 0, TENT);
  game_set_square(g3, 0, 2, TENT);

  if (game_won(g3)) ok = false;
  game_delete(g3);

  // Regle 4 Tente ajacente a un arbre

  // ⛺🟩
  // 🟩🌲

  game g4 = game_new_empty();
  game_set_expected_nb_tents_row(g4, 0, 1);
  game_set_expected_nb_tents_col(g4, 0, 1);

  game_set_square(g4, 1, 1, TREE);
  game_set_square(g4, 0, 0, TENT);

  if (game_won(g4)) ok = false;
  game_delete(g4);

  // NOUVEAU : Arbre grand = 2 arbres donc 2 tentes

  // ⛺🌳🟩🟩 1
  // 🟩🟩⛺🌲 1
  // 🟩🟩🟩🟩 0
  // 1 0 1 0

  game g5 = game_new_empty_ext(3, 4, true);
  game_set_expected_nb_tents_col(g5, 0, 1);
  game_set_expected_nb_tents_col(g5, 2, 1);
  game_set_expected_nb_tents_row(g5, 0, 1);
  game_set_expected_nb_tents_row(g5, 1, 1);
  game_set_square(g5, 0, 0, TENT);
  game_set_square(g5, 0, 1, BIGTREE);
  game_set_square(g5, 1, 2, TENT);
  game_set_square(g5, 1, 3, TREE);

  if (game_won(g5)) ok = false;
  game_delete(g5);

  // NOUVEAU : Grille wrap

  // ⛺🟩🌲 1
  // 🟩🟩🟩 0
  // 🟩🟩🟩 0
  // 1 0 0

  game g6 = game_new_empty_ext(3, 3, true);
  game_set_expected_nb_tents_col(g6, 0, 1);
  game_set_expected_nb_tents_row(g6, 0, 1);
  game_set_square(g6, 0, 0, TENT);
  game_set_square(g6, 0, 2, TREE);

  if (!game_won(g6)) ok = false;
  game_delete(g6);

  // Cas gagnant

  game g7 = game_default_ext();
  // On place les tentes
  game_set_square(g7, 0, 4, TENT);
  game_set_square(g7, 1, 1, TENT);
  game_set_square(g7, 2, 4, TENT);
  game_set_square(g7, 3, 1, TENT);

  if (!game_won(g7)) ok = false;
  game_delete(g7);

  // Cas BIGTREE considéré comme TREE

  // 🟩⛺🌳⛺🌲 2
  // 🟩🟩🟩🟩🟩 0
  // 🟩🟩🟩🟩🟩 0
  // 0  1 0  0 1

  game g8 = game_new_empty_ext(3, 5, false);
  game_set_expected_nb_tents_row(g8, 0, 2);
  game_set_expected_nb_tents_col(g8, 1, 1);
  game_set_expected_nb_tents_col(g8, 4, 1);
  game_set_square(g8, 0, 1, TENT);
  game_set_square(g8, 0, 2, BIGTREE);
  game_set_square(g8, 0, 3, TENT);
  game_set_square(g8, 0, 4, TREE);
  if (game_won(g8)) ok = false;
  game_delete(g8);

  // CAS BIGTREE seul dans la grille avec une tente
  // 🌳⛺🟩 1
  // 🟩🟩🟩 0
  // 🟩🟩🟩 0
  // 0 1 0

  game g10 = game_new_empty_ext(3, 3, false);
  game_set_expected_nb_tents_row(g10, 0, 1);
  game_set_expected_nb_tents_col(g10, 1, 1);
  game_set_square(g10, 0, 0, BIGTREE);
  game_set_square(g10, 0, 1, TENT);
  if (game_won(g10)) ok = false;
  game_delete(g10);

  // CAS BIGTREE seul dans la grille avec 3 tentes donc pas exactement 2
  // 🌳⛺🟩⛺ 2
  // 🟩🟩🟩🟩 0
  // 🟩🟩🟩🟩 0
  // ⛺🟩🟩🟩 1
  // 1 1 0 1

  game g11 = game_new_empty_ext(4, 4, true);
  game_set_expected_nb_tents_row(g11, 0, 2);
  game_set_expected_nb_tents_col(g11, 0, 1);
  game_set_expected_nb_tents_col(g11, 1, 1);
  game_set_expected_nb_tents_col(g11, 3, 1);
  game_set_square(g11, 0, 0, BIGTREE);
  game_set_square(g11, 0, 1, TENT);
  game_set_square(g11, 0, 3, TENT);
  game_set_square(g11, 3, 0, TENT);
  if (game_won(g11)) ok = false;
  game_delete(g11);

  // ⛺🟩🟩 2
  // 🟩🌳🟩 0
  // 🟩🟩⛺ 0
  // 1 0 1

  game g12 = game_new_empty_ext(3, 3, false);
  game_set_expected_nb_tents_row(g12, 0, 2);
  game_set_expected_nb_tents_col(g12, 0, 1);
  game_set_expected_nb_tents_col(g12, 2, 1);
  game_set_square(g12, 1, 1, BIGTREE);
  game_set_square(g12, 0, 0, TENT);
  game_set_square(g12, 2, 2, TENT);
  if (game_won(g12)) ok = false;
  game_delete(g12);

  // 🟩🟩🟩 0
  // ⛺🌳⛺ 2
  // 🟩🟩🟩 0
  // 1 0 1

  game g13 = game_new_empty_ext(3, 3, false);
  game_set_expected_nb_tents_row(g13, 1, 2);
  game_set_expected_nb_tents_col(g13, 0, 1);
  game_set_expected_nb_tents_col(g13, 2, 1);
  game_set_square(g13, 1, 1, BIGTREE);
  game_set_square(g13, 1, 0, TENT);
  game_set_square(g13, 1, 2, TENT);
  if (!game_won(g13)) ok = false;
  game_delete(g13);

  game g14 = game_new_empty_ext(3, 3, false);
  game_set_expected_nb_tents_row(g14, 1, 2);
  game_set_expected_nb_tents_col(g14, 0, 1);
  game_set_expected_nb_tents_col(g14, 2, 1);
  game_set_square(g14, 1, 1, BIGTREE);
  game_set_square(g14, 0, 1, TREE);
  game_set_square(g14, 1, 0, TENT);
  game_set_square(g14, 1, 2, TENT);
  if (game_won(g14)) ok = false;
  game_delete(g14);

  return ok;
}

//==========> GET AJACENT SQUARE <==========//

bool test_get_adjacent_square(void) {
  game g = game_default();
  bool ok = true;
  uint i_next, j_next;

  // Cas hors de la grid
  if (game_get_adjacent_square(g, 0, 0, UP_LEFT, &i_next, &j_next)) ok = false;
  if (game_get_adjacent_square(g, 0, 0, UP, &i_next, &j_next)) ok = false;
  if (game_get_adjacent_square(g, 0, 7, UP_RIGHT, &i_next, &j_next)) ok = false;
  if (game_get_adjacent_square(g, 0, 7, RIGHT, &i_next, &j_next)) ok = false;
  if (game_get_adjacent_square(g, 7, 7, DOWN_RIGHT, &i_next, &j_next)) ok = false;
  if (game_get_adjacent_square(g, 7, 0, DOWN, &i_next, &j_next)) ok = false;
  if (game_get_adjacent_square(g, 7, 0, DOWN_LEFT, &i_next, &j_next)) ok = false;
  if (game_get_adjacent_square(g, 0, 0, LEFT, &i_next, &j_next)) ok = false;

  // Cas dans la grid
  if (!game_get_adjacent_square(g, 4, 4, UP_LEFT, &i_next, &j_next)) ok = false;
  if (i_next != 3 || j_next != 3) ok = false;
  if (!game_get_adjacent_square(g, 4, 4, UP, &i_next, &j_next)) ok = false;
  if (i_next != 3 || j_next != 4) ok = false;
  if (!game_get_adjacent_square(g, 4, 4, UP_RIGHT, &i_next, &j_next)) ok = false;
  if (i_next != 3 || j_next != 5) ok = false;
  if (!game_get_adjacent_square(g, 4, 4, RIGHT, &i_next, &j_next)) ok = false;
  if (i_next != 4 || j_next != 5) ok = false;
  if (!game_get_adjacent_square(g, 4, 4, DOWN_RIGHT, &i_next, &j_next)) ok = false;
  if (i_next != 5 || j_next != 5) ok = false;
  if (!game_get_adjacent_square(g, 4, 4, DOWN, &i_next, &j_next)) ok = false;
  if (i_next != 5 || j_next != 4) ok = false;
  if (!game_get_adjacent_square(g, 4, 4, DOWN_LEFT, &i_next, &j_next)) ok = false;
  if (i_next != 5 || j_next != 3) ok = false;
  if (!game_get_adjacent_square(g, 4, 4, LEFT, &i_next, &j_next)) ok = false;
  if (i_next != 4 || j_next != 3) ok = false;

  game_delete(g);

  // Cas wrap
  game g2 = game_new_empty_ext(5, 5, true);

  game_get_adjacent_square(g2, 0, 0, UP_LEFT, &i_next, &j_next);
  if (i_next != 4 || j_next != 4) ok = false;
  game_get_adjacent_square(g2, 0, 0, UP, &i_next, &j_next);
  if (i_next != 4 || j_next != 0) ok = false;
  game_get_adjacent_square(g2, 0, 4, UP_RIGHT, &i_next, &j_next);
  if (i_next != 4 || j_next != 0) ok = false;
  game_get_adjacent_square(g2, 0, 4, RIGHT, &i_next, &j_next);
  if (i_next != 0 || j_next != 0) ok = false;
  game_get_adjacent_square(g2, 4, 4, DOWN_RIGHT, &i_next, &j_next);
  if (i_next != 0 || j_next != 0) ok = false;
  game_get_adjacent_square(g2, 4, 0, DOWN, &i_next, &j_next);
  if (i_next != 0 || j_next != 0) ok = false;
  game_get_adjacent_square(g2, 4, 0, DOWN_LEFT, &i_next, &j_next);
  if (i_next != 0 || j_next != 4) ok = false;
  game_get_adjacent_square(g2, 0, 0, LEFT, &i_next, &j_next);
  if (i_next != 0 || j_next != 4) ok = false;

  game_delete(g2);
  return ok;
}

//==========> NB ROWS <==========//

bool test_nb_rows(void) {
  bool ok = true;

  game g = game_default();  // Cas pour la game par defaut
  if (game_nb_rows(g) != 8) ok = false;
  game_delete(g);

  game g2 = game_new_empty_ext(10, 5, true);  // Cas pour une game à 10 lignes
  if (game_nb_rows(g2) != 10) ok = false;
  game_delete(g2);

  return ok;
}

//==========> NB COLS <==========//

bool test_nb_cols(void) {
  bool ok = true;

  game g = game_default();  // Cas pour la game par defaut
  if (game_nb_cols(g) != 8) ok = false;
  game_delete(g);

  game g2 = game_new_empty_ext(5, 10, true);  // Cas pour une game à 10 colonnes
  if (game_nb_cols(g2) != 10) ok = false;
  game_delete(g2);

  return ok;
}

//==========> IS WRAPPING <==========//

bool test_is_wrapping(void) {
  bool ok = true;

  game g = game_default();  // Cas pour la game par defaut
  if (game_is_wrapping(g)) ok = false;
  game_delete(g);

  game g2 = game_new_empty_ext(5, 4, false);  // Cas pour une game non wrap
  if (game_is_wrapping(g2)) ok = false;
  game_delete(g2);

  game g3 = game_new_empty_ext(5, 4, true);  // Cas pour une game wrap
  if (!game_is_wrapping(g3)) ok = false;
  game_delete(g3);

  return ok;
}

//==========> USAGE <==========//

void usage(int argc, char *argv[]) {
  fprintf(stderr, "Usage: %s <testname> [<...>]\n", argv[0]);
  exit(EXIT_FAILURE);
}

//==========> MAIN <==========//

int main(int argc, char *argv[]) {
  if (argc == 1) usage(argc, argv);

  fprintf(stderr, "=> Start test \"%s\"\n", argv[1]);
  bool ok = false;

  if (strcmp(argv[1], "dummy") == 0)
    ok = test_dummy();
  else if (strcmp("get_square", argv[1]) == 0)
    ok = test_get_square();
  else if (strcmp("set_square", argv[1]) == 0)
    ok = test_set_square();
  else if (strcmp("get_current_nb_tents_row", argv[1]) == 0)
    ok = test_get_current_nb_tents_row();
  else if (strcmp("get_current_nb_tents_col", argv[1]) == 0)
    ok = test_get_current_nb_tents_col();
  else if (strcmp("get_current_nb_tents_all", argv[1]) == 0)
    ok = test_get_current_nb_tents_all();
  else if (strcmp("check_move", argv[1]) == 0)
    ok = test_check_move();
  else if (strcmp("play_move", argv[1]) == 0)
    ok = test_play_move();
  else if (strcmp("won", argv[1]) == 0)
    ok = test_won();
  else if (strcmp("get_adjacent_square", argv[1]) == 0)
    ok = test_get_adjacent_square();
  else if (strcmp("nb_rows", argv[1]) == 0)
    ok = test_nb_rows();
  else if (strcmp("nb_cols", argv[1]) == 0)
    ok = test_nb_cols();
  else if (strcmp("is_wrapping", argv[1]) == 0)
    ok = test_is_wrapping();
  else {
    fprintf(stderr, "Error: test \"%s\" not found!\n", argv[1]);
    exit(EXIT_FAILURE);
  }
  if (ok) {
    fprintf(stderr, "Test \"%s\" finished: SUCCESS\n", argv[1]);
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "Test \"%s\" finished: FAILURE\n", argv[1]);
    return EXIT_FAILURE;
  }
}