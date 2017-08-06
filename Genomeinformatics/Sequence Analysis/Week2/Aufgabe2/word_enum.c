#include <stdlib.h>
#include <assert.h>
#include "word_enum.h"

struct Wordenumerator
{
  unsigned char *word;
  unsigned long length, numofwords, currentidx;
};

Wordenumerator *word_enumerator_new(unsigned long length)
{
  Wordenumerator *we = malloc(sizeof *we);

  assert(length > 0);
  we->word = malloc(sizeof *we->word * length);
  we->length = length;
  we->numofwords = 1UL << length;
  we->currentidx = 0;
  return we;
}

void word_enumerator_delete(Wordenumerator *we)
{
  if (we != NULL)
  {
    free(we->word);
    free(we);
  }
}

void word_enumerator_reset(Wordenumerator *we)
{
  we->currentidx = 0;
}

const unsigned char *word_enumerator_next(Wordenumerator *we)
{
  if (we->currentidx < we->numofwords)
  {
    unsigned long j, bits;

    for (j = 0, bits = 1UL << (we->length-1); bits > 0; bits >>= 1, j++)
    {
      assert(j < we->length);
      we->word[j] = (we->currentidx & bits) ? 'b' : 'a';
    }
    we->currentidx++;
    return we->word;
  }
  return NULL;
}
