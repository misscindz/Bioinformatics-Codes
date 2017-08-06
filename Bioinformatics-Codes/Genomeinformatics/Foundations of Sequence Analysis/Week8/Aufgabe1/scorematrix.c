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

#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include "linebuffer.h"
#include "alphabet.h"
#include "scorematrix.h"

struct Scorematrix
{
  Score **scoretab;
  Alphabet *alphabet;
};

void scorematrix_delete(Scorematrix *scorematrix)
{
  if (scorematrix != NULL)
  {
    free(scorematrix->scoretab[0]);
    free(scorematrix->scoretab);
    alphabet_delete(scorematrix->alphabet);
    free(scorematrix);
  }
}

/*
  The following function reads a score line of a score matrix.
*/
static int scanscoreline(Score *scoreline,
                         const unsigned char *line,
                         unsigned long linelen,
                         unsigned long linecount)
{
  unsigned long idx = 1, currentchar = 0;
  int readint;
  bool scanned = false;

  while (true)
  {
    if (idx==linelen)
    {
      break;
    }
    if (line[idx] == (unsigned char) ' ' || line[idx] == (unsigned char) '\t')
    {
      idx++;
      scanned = false;
    } else
    {
      if (line[idx] == (unsigned char) '-' || isdigit((int) line[idx]))
      {
        if (!scanned)
        {
          if (sscanf((char *) (line+idx),"%d",&readint) != 1)
          {
            fprintf(stderr,"line %lu: corrupted line\n\"%s\"",
                          linecount,(char *) (line+idx));
            return -1;
          }
          scoreline[currentchar++] = (Score) readint;
          scanned = true;
        } else
        {
          idx++;
        }
      }
    }
  }
  return 0;
}

static Score **scoretab_new(unsigned long alphasize)
{
  unsigned long idx;
  Score **scoretab = malloc(sizeof(*scoretab) * alphasize);

  assert(scoretab != NULL);
  scoretab[0] = malloc(sizeof(**scoretab) * alphasize * alphasize);
  assert(scoretab[0] != NULL);
  for (idx = 1; idx < alphasize; idx++)
  {
    scoretab[idx] = scoretab[idx-1] + alphasize;
  }
  return scoretab;
}

#define LINE(I) line.space[I]

Scorematrix *scorematrix_new(const char *inputfile)
{
  unsigned long linecount = 0, currentscoreline = 0;
  Linebuffer line = {NULL,0,0};
  FILE *fpin;
  Scorematrix *scorematrix;
  bool haserr = false;

  fpin = fopen(inputfile,"rb");
  if (fpin == NULL)
  {
    fprintf(stderr,"cannot open file \"%s\"\n",inputfile);
    return NULL;
  }
  scorematrix = malloc(sizeof *scorematrix);
  assert(scorematrix != NULL);
  scorematrix->scoretab = NULL;
  scorematrix->alphabet = NULL;
  while (!haserr && linebuffer_next(fpin, &line) != EOF)
  {
    linecount++;
    if (line.nextfree > 0)
    {
      if (LINE(0) != (unsigned char) '#')
      {
        if (LINE(0) == (unsigned char) ' ')
        {
          scorematrix->alphabet = scorematrix_alphabet_new(line.space,
                                                           line.nextfree);
          scorematrix->scoretab
            = scoretab_new(alphabet_size(scorematrix->alphabet));
        } else
        {
          if (alphabet_get_char(scorematrix->alphabet,currentscoreline) 
              != LINE(0))
          {
            fprintf(stderr,"line %lu: first character must be %c",
                           linecount,
                           LINE(0));
            haserr = true;
          }
          if (!haserr && scanscoreline(scorematrix->scoretab[currentscoreline],
                                       line.space,
                                       line.nextfree,
                                       linecount) != 0)
          {
            haserr = true;
          } else
          {
            currentscoreline++;
          }
        }
      }
    }
    line.nextfree = 0;
  }
  fclose(fpin);
  free(line.space);
  if (haserr)
  {
    scorematrix_delete(scorematrix);
    return NULL;
  }
  return scorematrix;
}

void scorematrix_show(const Scorematrix *scorematrix)
{
  unsigned long i, j, alphasize;

  assert(scorematrix != NULL);
  alphasize = alphabet_size(scorematrix->alphabet);
  printf("   ");
  for (j = 0; j < alphasize; j++)
  {
    printf("%c",alphabet_get_char(scorematrix->alphabet,j));
    printf("%s",(j < alphasize - 1) ? "  " : "\n");
  }
  for (i = 0; i < alphasize; i++)
  {
    printf("%c ",alphabet_get_char(scorematrix->alphabet,i));
    for (j = 0; j < alphasize; j++)
    {
      printf("%2d",scorematrix->scoretab[i][j]);
      printf("%s",(j < alphasize - 1) ? " " : "\n");
    }
  }
}

Score scorematrix_score_get(const Scorematrix *scorematrix,unsigned char a,
                            unsigned char b)
{
#ifndef NDEBUG
  unsigned long alphasize;
#endif
  
  assert(scorematrix != NULL);
#ifndef NDEBUG
  alphasize = alphabet_size(scorematrix->alphabet);
#endif
  assert(a < alphasize && b < alphasize);
  return scorematrix->scoretab[a][b];
}

const Alphabet *scorematrix_alphabet(const Scorematrix *scorematrix)
{
  assert(scorematrix != NULL && scorematrix->alphabet != NULL);
  return scorematrix->alphabet;
}
