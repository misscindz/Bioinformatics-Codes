#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>
#include <limits.h>
#include <inttypes.h>
#include "outsenseedist-trim.h"

struct TrimParams
{
  unsigned long historysize,   /* user defined <= 64, default 60 */
                regionalquality, /* user defined <= historysize, default 33 */
                frontheadlag;  /* user defined, default 30 */
  uint64_t leftmostbit; /* no init necessary */
};

/* constructor for the type TrimParams */

TrimParams *trimparams_new(unsigned long historysize,
                           unsigned long minimalmatchpercentage,
                           unsigned long frontheadlag)
{
  TrimParams *trimparams = malloc(sizeof *trimparams);

  assert(trimparams != NULL);
  assert(historysize <= sizeof *trimparams * CHAR_BIT);
  assert(minimalmatchpercentage <= 100);
  trimparams->historysize = historysize;
  trimparams->regionalquality = historysize
                                * (double) minimalmatchpercentage/100;
  trimparams->frontheadlag = frontheadlag;
  printf("# historysize=%lu\n",trimparams->historysize);
  printf("# regionalquality=%lu\n",trimparams->regionalquality);
  printf("# frontheadlag=%lu\n",trimparams->frontheadlag);
  trimparams->leftmostbit = ((uint64_t) 1) << (historysize - 1);
  return trimparams;
}

/* destructor for the type TrimParams */

void trimparams_delete(TrimParams *trimparams)
{
  if (trimparams != NULL)
  {
    free(trimparams);
  }
}

/* The type of an element in the front table FE */

typedef struct
{
  unsigned int row;
  unsigned char matches, /* number of matches == #1's in bitvector */
                bitsused; /* number of bits actually used */
  uint64_t bitvector;  /* represents window of width historysize on the
                          previous columns of the alignment */
} TrimValue;

/* this function computes the lcp for the strings referenced
   by uptr and vptr. uend and vend point to the memory location _after_
   the end of the string */

__attribute__ ((unused))
static void add_matches(const TrimParams *trimparams,
                        TrimValue *best,
                        const unsigned char *uptr, const unsigned char *uend,
                        const unsigned char *vptr, const unsigned char *vend)
{
  /* to be implemented */
}

/* compute the destfront from the sourcefront. trimleft and valid
   represent the range of entries not trimmed, as described in the
   lecture notes. The trimming parameters are passed as the first
   argument. It return the largest sum of row and columns of all
   entries in the destfront, later called maxaligned. */

__attribute__ ((unused))
static unsigned long outsense_next_front_trim(const TrimParams *trimparams,
                                              TrimValue *destfront,
                                              const TrimValue *sourcefront,
                                              unsigned long trimleft,
                                              unsigned long valid,
                                              unsigned long d,
                                              const unsigned char *useq,
                                              unsigned long ulen,
                                              const unsigned char *vseq,
                                              unsigned long vlen)
{
  /* to be implemented */
  return 0;
}

/* decide whether this entry is trimmed. Note that the strategy
   based on the regional alignment quality only becomes effective
   one the number of bits actually used equals the historysize */

__attribute__ ((unused))
static bool trimthisentry(const TrimParams *trimparams,
                          const TrimValue *v,
                          long didx,
                          unsigned long maxaligned)
{
  if (v->bitsused == trimparams->historysize &&
      v->matches < trimparams->regionalquality)
  {
    return true;
  }
  if (2 * v->row + didx + trimparams->frontheadlag < maxaligned)
  {
    return true;
  }
  return false;
}

/* for debugging, the bitvector may be output. bits in the size of 
   the history */

#undef SHOWFRONT
#ifdef SHOWFRONT
static char *bitsequence_to_string(uint64_t bs,unsigned long bits)
{
  unsigned int idx;
  uint64_t mask;
  char *buffer;

  buffer = malloc(sizeof(*buffer) * (bits + 1));
  assert(buffer != NULL);
  for (idx = 0, mask = ((uint64_t) 1) << (bits - 1);
       mask > 0; idx++, mask >>= 1)
  {
    buffer[idx] = (bs & mask) ? '1' : '0';
  }
  assert(idx == bits);
  buffer[idx] = '\0';
  return buffer;
}
#endif

/* run the iteration with increasing value of d and return the 
   edist distance, if the last row and last column was reached.
   If at some point valid becomes 0 (we then say that the front has
   died_out), then return ulen + vlen + 1, which is one larger than the
   maximum unit edit distance. info is
   the pointer by which the TrimParams pointer is passed. So
   Declare and init a local pointer as follows
   TrimParams *trimparams = (TrimParams *) info;
*/

unsigned long outsense_edist_trim(void *info,
                                  const unsigned char *useq,
                                  unsigned long ulen,
                                  const unsigned char *vseq,
                                  unsigned long vlen)
{
  return 0;
}
