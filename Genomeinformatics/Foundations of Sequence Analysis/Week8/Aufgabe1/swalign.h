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

#ifndef SWALIGN_H
#define SWALIGN_H
#include "scorematrix.h"

typedef struct
{
  unsigned long
    ustart, /* start position of local opt. alignment in <useq> */
    usubstringlength, /* length of substring of local opt. align. in <useq> */
    vstart, /* start position of local opt. alignment in <vseq> */
    vsubstringlength; /* length of substring of local opt. align. in <vseq> */
  Score score; /* score of local optimal alignment */
} OptimalLocalAlignmentCoords;

/*
   For two sequences and a score function the following function implements
   the SmithWaterman Algorithm. It determines the coordinates of an optimal
   local alignment.
   The last arguments specify the input parameters, consisting of
   the scorematrix-object, insertion and deletion scores, references
   to the sequences and their length. The results is stored
   in the structure pointed to by <coords>, which specifies the
   coordinates of a pair of substrings in <useq> and <vseq> with a maximal
   global alignment score over all pairs of substrings. The semantics of
   the different components are specified in declaration of the structure
   above. To make the result deterministic, the following rule is applied:
   If two different entries (i,j) and (l,k) with the maximum score
   exist, then (i,j) is chosen if and only if l+k<i+j or l+k=i+j and
   k<j. In other words, from all entries with maximum score,
   the entry which maximizes the sum of row and
   column is chosen and from all entries with maximum score and maximum sum
   of row and column, the one with larger column is chosen. For a
   given maximum entry determined as described above, one has to compute
   the start coordinates (i',j') of the corresponding local alignment. This
   can be done by backtracing on a maximizing path until an entry with score
   0 is found. To make the coordinates unique, one prefers replacements
   over deletions and deletions over inseration in case there are is more
   than one incoming maximimzing edge. The backtracing approach, which would
   be a correct solution,  however requires O(mn) space. There is an
   alternative method which only requires O(m) space to determine (i',j').
   This solution is of course the preferred solution.
*/

void smithwaterman_algorithm(OptimalLocalAlignmentCoords *coords,
                             const Scorematrix *scorematrix,
                             Score insertionscore,
                             Score deletionscore,
                             const unsigned char *useq,
                             unsigned long ulen,
                             const unsigned char *vseq,
                             unsigned long vlen);


#endif
