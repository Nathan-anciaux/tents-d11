///////////////////////////
/// Primitive des piles ///
///////////////////////////

#ifndef STACK_H_
#define STACK_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"

///////////////////////////////////////////
// Définition du type, à ne pas modifier //
///////////////////////////////////////////

typedef struct {
  int row;
  int col;
  square square;
} setsquare;

typedef struct stack {
  int capacity;      // Taille totale du tableau alloué en mémoire.
  int size_stack;    // Nombre d'éléments dans la pile représentée.
  setsquare* array;  // Tableau contenant les valeurs de la pile représentée.
} stack;

///////////////////////////////////
// Fonctions primitives à écrire //
///////////////////////////////////

// Crée une pile vide.
stack* create_stack(void);

// Libère la mémoire allouée pour la pile.
void delete_stack(stack*);

// Renvoie vrai si la pile est vide, faux sinon.
bool isempty_stack(stack*);

// Renvoie la valeur au sommet de la pile et la retire. Si la pile est vide, la
// fonction affiche un message sur la sortie erreur standard et termine le
// programme. Si l'occupation du tableau tombe à 25% après le pop(), le tableau
// est redimensionné par la fonction shrink_stack().
setsquare pop(stack*);

// Ajoute une valeur au sommet de la pile. Si le tableau est plein, il est
// redimensionné au préalable, par la fonction grow_stack.
void push(setsquare, stack*);

#endif  // STACK_H_
