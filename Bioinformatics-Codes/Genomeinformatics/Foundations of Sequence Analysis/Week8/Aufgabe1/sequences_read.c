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
#include <ctype.h>
#include <assert.h>
#include <string.h>
#include "linebuffer.h"
#include "sequences_read.h"

typedef struct
{
  unsigned char *content;
  unsigned long length;
} SequenceInfo;

static void sequence_info_save(SequenceInfo *siptr,const unsigned char *s,
                               unsigned long length)
{
  unsigned long widx, ridx;

  siptr->content = malloc(sizeof *siptr->content * length);
  for (ridx = 0, widx = 0; ridx < length; ridx++)
  {
    if (!isspace(s[ridx]))
    {
      siptr->content[widx++] = s[ridx];
    }
  }
  siptr->length = widx;
}

typedef struct
{
  SequenceInfo *sequence_info;
  unsigned long numofsequences;
} ArraySequences;

static ArraySequences *sequences_new(FILE *fpin)
{
  Linebuffer linebuffer = {NULL,0,0};
  ArraySequences *arr = malloc(sizeof *arr);
  unsigned long allocatedSequenceInfo = 0;

  arr->sequence_info = NULL;
  arr->numofsequences = 0;
  while (linebuffer_next(fpin,&linebuffer) != EOF)
  {
    if (arr->numofsequences >= allocatedSequenceInfo)
    {
      allocatedSequenceInfo = (allocatedSequenceInfo * 1.2) + 64;
      arr->sequence_info
        = realloc(arr->sequence_info,sizeof *arr->sequence_info *
                                     allocatedSequenceInfo);
    }
    sequence_info_save(arr->sequence_info + arr->numofsequences,
                       linebuffer.space,linebuffer.nextfree);
    arr->numofsequences++;
    linebuffer.nextfree = 0;
  }
  free(linebuffer.space);
  return arr;
}

static void sequences_delete(ArraySequences *arr)
{
  if (arr != NULL)
  {
    unsigned long idx;

    for (idx = 0; idx < arr->numofsequences; idx++)
    {
      free(arr->sequence_info[idx].content);
    }
    free(arr->sequence_info);
    free(arr);
  }
}

void sequences_show(const ArraySequences *arr)
{
  if (arr != NULL)
  {
    unsigned long idx;

    for (idx = 0; idx < arr->numofsequences; idx++)
    {
      fwrite(arr->sequence_info[idx].content,sizeof (char),
             arr->sequence_info[idx].length,stdout);
      printf("\n");
    }
  }
}

static const unsigned char *sequences_content_get(const ArraySequences *arr,
                                                  unsigned long idx)
{
  assert(idx < arr->numofsequences);
  return arr->sequence_info[idx].content;
}

static unsigned long sequences_length_get(const ArraySequences *arr,
                                          unsigned long idx)
{
  assert(idx < arr->numofsequences);
  return arr->sequence_info[idx].length;
}

static unsigned long sequences_numofsequences(const ArraySequences *arr)
{
  return arr->numofsequences;
}

struct SequencePairEnumerator
{
  unsigned long idx_u, idx_v, numofsequences;
  unsigned char *tuseq, *tvseq;
  ArraySequences *arr;
  const Alphabet *alphabet;
};

SequencePairEnumerator *sequence_pair_enumerator_new(const char *inputfile,
                                                     const Alphabet *alphabet)
{
  unsigned long idx, maxlength = 0;
  SequencePairEnumerator *spe;
  FILE *fpin = fopen(inputfile,"r");

  if (fpin == NULL)
  {
    fprintf(stderr,"%s: cannot open file \"%s\"\n",__func__,inputfile);
    return NULL;
  }
  spe = malloc(sizeof *spe);
  assert(spe != NULL);
  spe->arr = sequences_new(fpin);
  fclose(fpin);
  spe->numofsequences = sequences_numofsequences(spe->arr);
  for (idx = 0; idx < spe->numofsequences; idx++)
  {
    unsigned long len = sequences_length_get(spe->arr,idx);
    if (maxlength < len)
    {
      maxlength = len;
    }
  }
  spe->idx_u = 0;
  spe->idx_v = 1;
  spe->tuseq = malloc(sizeof *spe->tuseq * maxlength);
  spe->tvseq = malloc(sizeof *spe->tvseq * maxlength);
  spe->alphabet = alphabet;
  return spe;
}

void sequence_pair_enumerator_delete(SequencePairEnumerator *spe)
{
  if (spe != NULL)
  {
    free(spe->tuseq);
    free(spe->tvseq);
    sequences_delete(spe->arr);
    free(spe);
  }
}

bool sequence_pair_enumerator_next(SequencePair *seqpair,
                                   SequencePairEnumerator *spe)
{
  if (spe->idx_u < spe->numofsequences && spe->idx_v < spe->numofsequences)
  {
    const unsigned char *useq = sequences_content_get(spe->arr,spe->idx_u);
    const unsigned char *vseq = sequences_content_get(spe->arr,spe->idx_v);

    seqpair->ulen = sequences_length_get(spe->arr,spe->idx_u);
    seqpair->vlen = sequences_length_get(spe->arr,spe->idx_v);
    seqpair->tuseq = spe->tuseq;
    seqpair->tvseq = spe->tvseq;
    seqpair->idx_u = spe->idx_u;
    seqpair->idx_v = spe->idx_v;
    memcpy(spe->tuseq,useq,seqpair->ulen);
    alphabet_transform_sequence_inplace(spe->alphabet,spe->tuseq,seqpair->ulen);
    memcpy(spe->tvseq,vseq,seqpair->vlen);
    alphabet_transform_sequence_inplace(spe->alphabet,spe->tvseq,seqpair->vlen);
    if (spe->idx_v + 1 == spe->numofsequences)
    {
      spe->idx_u++;
      spe->idx_v = spe->idx_u + 1;
    } else
    {
      spe->idx_v++;
    }
    return true;
  } else
  {
    return false;
  }
}
