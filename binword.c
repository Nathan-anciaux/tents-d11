// @brief Generate binary words.
// @author <aurelien.esnard@u-bordeaux.fr>

#define N 4

#include <stdio.h>
#include <stdlib.h>

void printWord(int* word, int len)
{
  for (int i = 0; i < len; i++) printf("%d", word[i]);
  printf("\n");
}

void genWords(int len, int* word, unsigned long* count)
{
  for (int a = 0; a <= 1; a++)
    for (int b = 0; b <= 1; b++)
      for (int c = 0; c <= 1; c++)
        for (int d = 0; d <= 1; d++) {
          word[0] = a;
          word[1] = b;
          word[2] = c;
          word[3] = d;
          (*count)++;
          printWord(word, len);
        }
}

int main(int argc, char* argv[])
{
  int word[N];
  unsigned long nbwords = 0;

  // reset word
  for (int k = 0; k < N; k++) word[k] = 0;

  // generate all possible words of size N=4
  genWords(N, word, &nbwords);

  printf("=> %lu words generated (of length %d)\n", nbwords, N);

  return EXIT_SUCCESS;
}