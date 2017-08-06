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

#ifndef SEQUENCES_READ_H
#define SEQUENCES_READ_H
#include <stdio.h>
#include <stdbool.h>
#include "alphabet.h"

typedef struct
{
  unsigned long ulen, vlen, idx_u, idx_v;
  const unsigned char *tuseq, *tvseq;
} SequencePair;

typedef struct SequencePairEnumerator SequencePairEnumerator;

SequencePairEnumerator *sequence_pair_enumerator_new(const char *inputfile,
                                                     const Alphabet *alphabet);

bool sequence_pair_enumerator_next(SequencePair *seqpair,
                                   SequencePairEnumerator *spe);

void sequence_pair_enumerator_delete(SequencePairEnumerator *spe);

#endif
