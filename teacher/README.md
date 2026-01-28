# README

## Rules

This game is a simple one-player puzzle. The game starts with a grid of squares,
some of which contain *trees* (🌲). The goal of the game is to place *tents* (⛺)
in the *empty* squares (⬜) in such a way that the following conditions are met:

- Rule 1) No two tents are adjacent, even diagonally.
- Rule 2) The number of tents in each row, and in each column, matches the
  expected numbers given around the sides of the grid.
- Rule 3) There are exactly as many tents as trees.
- Rule 4) Each tent must be orthogonally adjacent (horizontally or vertically,
  but not diagonally) to at least one tree and vice versa.

Besides, it is also possible to place *grass* (🟩) in a square, indicating that
you are sure there is no tent on this square. The *grass* square gives a clue,
but it is not necessary to use it to win the game.

In summary, the game uses 4 different squares:

- empty (⬜ or ' ')
- tree (🌲 or 'x')
- tent (⛺ or '*')
- grass (🟩 or '-')

By convention, the square in the *i*-th row and *j*-th column of the grid is
referred to as the coordinate (i,j), and the coordinate (0,0) corresponds to the
top left corner. The dimension of the default game is 8x8.

You can try the original game on the famous <a
href="https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/tents.html"> Simon
Tatham's Portable Puzzle Collection</a>


## Example: Default Game

As an example, let's see the instance of the game, considered thereafter as the
*default* game:

```
🌲⬜⬜⬜⬜🌲🌲⬜ 1
⬜⬜⬜⬜⬜⬜⬜⬜ 2
⬜⬜⬜⬜⬜⬜🌲⬜ 1
🌲⬜🌲⬜⬜⬜⬜⬜ 2
⬜⬜⬜⬜🌲⬜⬜⬜ 2
⬜⬜🌲⬜⬜⬜⬜⬜ 0
⬜⬜⬜⬜⬜⬜⬜⬜ 1
⬜🌲🌲⬜⬜🌲🌲⬜ 3
 3 0 2 1 2 1 2 1
```

Hereafter, its solution:

```
🌲🟩🟩🟩⛺🌲🌲🟩 1
⛺🟩🟩🟩🟩🟩⛺🟩 2
🟩🟩⛺🟩🟩🟩🌲🟩 1
🌲🟩🌲🟩⛺🟩⛺🟩 2
⛺🟩⛺🟩🌲🟩🟩🟩 2
🟩🟩🌲🟩🟩🟩🟩🟩 0
🟩🟩🟩🟩🟩⛺🟩🟩 1
⛺🌲🌲⛺🟩🌲🌲⛺ 3
 3 0 2 1 2 1 2 1
```

Try it: https://www.chiark.greenend.org.uk/~sgtatham/puzzles/js/tents.html#8x8de#911105660705777

## About the Rule 4

Note that the Rule 4) is not exactly formulated in the same way in the original
game. According to <a
href="https://www.chiark.greenend.org.uk/~sgtatham/puzzles/doc/tents.html">Simon
Tatham</a>, the rule 4) is defined by a *matching* condition:

> It is possible to match tents to trees so that each tree is orthogonally
> adjacent to its own tent (but may also be adjacent to other tents).

Another way of phrasing the tree/tent matching criterion is to say that the
bipartite adjacency graph between trees and tents has a perfect matching. And
there is no trivial algorithm to check this condition (see <a
href="https://en.wikipedia.org/wiki/Hopcroft%E2%80%93Karp_algorithm">
Hopcroft–Karp algorithm</a>).

Here is an example that satisfies the Rule 4) as we have stated it, but does not
satisfy the *matching criterion* formulated in the original rule.

```
⛺⬜🌲
🌲⬜⛺
⛺⬜🌲
```

The variant of the Rule 4) we have chosen is a local criterion, that is much easier to implement.

## Game Extension

We will now consider a new version of the game (*version 2*), which is defined by the following changes:

* The grid can now be rectangular and have any arbitrary number of rows and columns.
* The game grid can optionally have a *toric* topology (the `wrapping` option): the rightmost column is adjacent to the leftmost column, and the top row is adjacent to the bottom row. 
* In addition, we add a new piece, `BIGTREE` (represented by '#'), to the `square` enumeration in [game.h](misc/game.h). This piece represents a *big tree* that must be assigned exactly 2 tents, whereas the `TREE` piece accepts (at least) one tent. Actually, it is not possible for a tree to accept more than 2 tents because of the adjacency rules.
* We also introduce move history management via `game_play_move()`, allowing the last moves played to be undone (`game_undo()`), and possibly redone (`game_redo()`). Note that you can redo as many moves as you have undone consecutively. However, you can no longer redo past moves once a new move has been played.

The file @ref game_ext.h contains the description of these new features. Note that version 2 of the game keeps the compatibility with version 1.


## Programming Interface

The game interface is divided into 2 files:

- Basic Game Functions: @ref game.h
- Auxiliarry Game Functions: @ref game_aux.h
- Extended Game Functions: @ref game_ext.h

It manipulates the *opaque* @ref game structure by means of pointers or const pointers (@ref cgame).

- https://en.wikipedia.org/wiki/Opaque_data_type

About Const Pointer:

- See also: http://www.geeksforgeeks.org/const-qualifier-in-c/
- See also this more technical discussion: http://stackoverflow.com/questions/8504411/typedef-pointer-const-weirdness

## Error Policy

There are many different ways to deal with program errors... Concerning the
"game" library, we suppose that function calls to these interfaces (*game.h*,
...) must systematically be made with valid arguments (see preconditions). Also,
the behavior of the library is not defined if a third-party program passes bad
arguments to it. In practice, this means that the responsibility of checking
that the arguments are valid is relieved to the high-level program (*game_text*,
...).

In order to debug your program easily, we recommand to use the `assert()` macro,
as it allows you to activate checks only in DEBUG mode and not in RELEASE mode.
More precisely, if you pass the `-DNDEBUG` option at compilation time, the macro
`assert()` generates no code, and hence does nothing at all.

For example:

```C
#include <assert.h>
#include <stdlib.h>

/** duplicate an array of integer and return it */
int* copyArray(int size, int * array)
{
  assert(size > 0);                           // check size parameter
  assert(array != NULL);                      // check array parameter
  int * newarray = malloc(size*sizeof(int));  // allocate memory for a new array
  assert(newarray != NULL);                   // check memory allocation
  for(int i = 0 ; i < size ; i++)             // copy array values
    newarray[i] = array[i];
  return newarray;
}
```
---
