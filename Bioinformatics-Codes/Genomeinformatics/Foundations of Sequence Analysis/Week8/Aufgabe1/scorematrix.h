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

#ifndef SCOREMATRIX_H
#define SCOREMATRIX_H
#include "alphabet.h"

typedef struct Scorematrix Scorematrix;

typedef int Score;

Scorematrix *scorematrix_new(const char *inputfile);
void scorematrix_delete(Scorematrix *scorematrix);
void scorematrix_show(const Scorematrix *scorematrix);
Score scorematrix_score_get(const Scorematrix *scorematrix,unsigned char a,
                            unsigned char b);

const Alphabet *scorematrix_alphabet(const Scorematrix *scorematrix);

#endif
