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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "scorematrix.h"
#include "sequences_read.h"
#include "swalign.h"

/* The following is only used for test purpose and not called here */

static int all_against_all_sw(const Scorematrix *scorematrix,
                              Score insertionscore,
                              Score deletionscore,
                              const char *inputfile)
{
  const Alphabet *alphabet = scorematrix_alphabet(scorematrix);
  SequencePair seqpair;
  OptimalLocalAlignmentCoords coords;
  SequencePairEnumerator *spe 
    = sequence_pair_enumerator_new(inputfile,alphabet);

  if (spe == NULL)
  {
    return -1;
  }
  while (sequence_pair_enumerator_next(&seqpair,spe))
  {
    smithwaterman_algorithm(&coords,
                            scorematrix,
                            insertionscore,
                            deletionscore,
                            seqpair.tuseq,
                            seqpair.ulen,
                            seqpair.tvseq,
                            seqpair.vlen);
    printf("%lu\t%lu\t",seqpair.idx_u,seqpair.idx_v);
    printf("%lu\t%lu\t%lu\t%lu\t%d",coords.ustart,
                                    coords.usubstringlength,
                                    coords.vstart,
                                    coords.vsubstringlength,
                                    coords.score);

    printf("\n");
  }
  sequence_pair_enumerator_delete(spe);
  return 0;
}

int main(int argc,char *argv[])
{
  Scorematrix *scorematrix;
  Score insertionscore, deletionscore;
  char *scorematrixfile;
  bool haserr = false;

  if (argc != 4) {
    fprintf(stderr,"Usage: %s indelscore scorematrix inputfile\n",argv[0]);
    return EXIT_FAILURE;
  }
  if (sscanf(argv[1],"%d",&insertionscore) != 1) {
    fprintf(stderr,"%s: second argument must be integer\n",argv[0]);
    return EXIT_FAILURE;
  }
  deletionscore = insertionscore;
  scorematrixfile = argv[2];
  scorematrix = scorematrix_new(scorematrixfile);
  if (scorematrix == NULL)
  {
    haserr = true;
  }
  if (!haserr)
  {
    const char *inputfile = argv[3];
    if (all_against_all_sw(scorematrix,insertionscore,deletionscore,
                           inputfile) != 0)
    {
      haserr = true;
    }
    scorematrix_delete(scorematrix);
  }
  return haserr ? EXIT_FAILURE : EXIT_SUCCESS;
}
