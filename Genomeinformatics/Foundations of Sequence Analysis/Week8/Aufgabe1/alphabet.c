/*
  Copyright (c) 2016 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2016 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*
  Copyright (c) 2016 Stefan Kurtz <kurtz@zbh.uni-hamburg.de>
  Copyright (c) 2016 Center for Bioinformatics, University of Hamburg

  Permission to use, copy, modify, and distribute this software for any
  purpose with or without fee is hereby granted, provided that the above
  copyright notice and this permission notice appear in all copies.

  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include "alphabet.h"

struct Alphabet
{
  unsigned char characters[UCHAR_MAX+1];/* list of characters in the alphabet */
  unsigned long nextfree,                /* nextfree entry in the alphabet */
                alphasize;               /* size of domain of symbolmap = 
                                           \#characters */
  unsigned char undefsymbol,
                symbolmap[UCHAR_MAX+1];  /* mapping of the symbols */
};

Alphabet *scorematrix_alphabet_new(const unsigned char *line,
                                   unsigned long linelen)
{
  unsigned long read_idx, write_idx;
  Alphabet *alpha = malloc(sizeof *alpha);

  assert(alpha != NULL);
  for (write_idx = read_idx = 0; read_idx < linelen; read_idx++)
  {
    if (!isspace(line[read_idx]))
    {
      alpha->characters[write_idx++] = (unsigned char) line[read_idx];
    }
  }
  alpha->characters[write_idx] = (unsigned char) '\0';
  alpha->alphasize = write_idx;
  alpha->undefsymbol = (unsigned char) UCHAR_MAX;
  for (read_idx = 0; read_idx <= UCHAR_MAX; read_idx++)
  {
    alpha->symbolmap[read_idx] = alpha->undefsymbol;
  }
  for (read_idx=0; read_idx<alpha->alphasize; read_idx++)
  {
    alpha->symbolmap[(unsigned long) alpha->characters[read_idx]] = read_idx;
  }
  return alpha;
}

void alphabet_delete(Alphabet *alphabet)
{
  if (alphabet != NULL)
  {
    free(alphabet);
  }
}

unsigned long alphabet_size(const Alphabet *alphabet)
{
  assert(alphabet != NULL);
  return alphabet->alphasize;
}

unsigned char alphabet_get_char(const Alphabet *alphabet, unsigned long idx)
{
  assert(alphabet != NULL && idx < alphabet->alphasize);
  return alphabet->characters[idx];
}

int alphabet_transform_sequence_inplace(const Alphabet *alphabet,
                                        unsigned char *sequence,
                                        unsigned long len)
{
  unsigned long idx;

  for (idx = 0; idx < len; idx++)
  {
    unsigned char cc = sequence[idx];

    if (alphabet->symbolmap[cc] == alphabet->undefsymbol)
    {
      return -1;
    }
    sequence[idx] = alphabet->symbolmap[cc];
  }
  return 0;
}
