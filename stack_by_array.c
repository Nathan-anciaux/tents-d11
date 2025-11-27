
#include "stack.h"

////////////////////////////////////////////////////////////////////////////////
// Fonctions auxiliaires ne pouvant pas être utilisées dans d'autres fichiers.//
// Elles ne sont PAS déclarées dans stack.h et sont déclarées "static". Cela  //
// signifie qu'elles ne peuvent être utilisées QUE dans ce fichier, stack.c.  //
////////////////////////////////////////////////////////////////////////////////

// Double la taille du tableau utilisé dans la représentation.
// Cette fonction sera utilisée lorsque le tableau est plein et qu'on veut y
// ajouter une valeur.
static void grow_stack(stack *p) {
  p->array = realloc(p->array, 2 * p->capacity * (sizeof(int)));
  p->capacity *= 2;
  return;
}

// Divise par deux la taille du tableau utilisé dans la représentation.
// Cette fonction sera utilisée lorsque le tableau est rempli à moins de 25%
// de sa capacité.
static void shrink_stack(stack *p) {
  if (p->capacity == 1) {
    printf("the table is too short !");
    return;
  }
  p->array = realloc(p->array, p->capacity / 2 * sizeof(int));
  p->capacity /= 2;
  return;
}

////////////////////////////////////////////////////////////////
// Fonctions à écrire, documentées dans le fichier d'en-tête. //
////////////////////////////////////////////////////////////////

stack *create_stack(void) {
  stack *p = malloc(sizeof(stack));
  p->capacity = 1;
  p->size_stack = 0;
  p->array = malloc((p->capacity) * sizeof(int));
  return p;
}

void delete_stack(stack *p) {
  free(p->array);
  free(p);
  return;
}

bool isempty_stack(stack *p) { return p->size_stack == 0; }

setsquare pop(stack *p) {
  if (isempty_stack(p)) {
    fprintf(stderr, "the table is too short ! \n");
    exit(EXIT_FAILURE);
  }
  p->size_stack--;
  setsquare result = p->array[p->size_stack];
  if ((p->size_stack * 100 / p->capacity) < 25) {
    shrink_stack(p);
  }
  return result;
}

void push(setsquare val, stack *p) {
  if (p->capacity == p->size_stack) {
    grow_stack(p);
  }
  p->array[p->size_stack] = val;
  p->size_stack = p->size_stack + 1;
  return;
}