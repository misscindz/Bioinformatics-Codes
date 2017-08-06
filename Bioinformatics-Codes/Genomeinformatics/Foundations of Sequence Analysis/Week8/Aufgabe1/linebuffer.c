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

#include <stdlib.h>
#include <stdbool.h>
#include "linebuffer.h"

/*
  The following functions read the next line from the given file pointer
  and stores it in the Linebuffer \texttt{line}.
*/

int linebuffer_next(FILE *fpin, Linebuffer *line)
{
  int cc;

  while (true)
  {
    cc = fgetc(fpin);
    if (cc == EOF)
    {
      return EOF;
    }
    if (line->nextfree >= line->allocated)
    {
      line->allocated += 512;
      line->space = realloc(line->space,
                            sizeof (*line->space) * line->allocated);
      if (line->space == NULL)
      {
        fprintf(stderr,"realloc failed\n");
        return -1;
      }
    }
    if (cc == '\n')
    {
      line->space[line->nextfree] = '\0';
      return 0;
    }
    line->space[line->nextfree++] = (unsigned char) cc;
  }
}











