#include <stdlib.h>
#include <assert.h>

#define array2dim_malloc(ARR2DIM, ROWS, COLS)\
        {\
          unsigned long idx;\
          ARR2DIM = malloc(sizeof (*ARR2DIM) * (ROWS));\
          assert(ARR2DIM != NULL);\
          (ARR2DIM)[0]\
            = malloc(sizeof (**ARR2DIM) * (ROWS) * (COLS));\
          assert(ARR2DIM[0] != NULL);\
          for (idx = 1UL; idx < (ROWS); idx++) {\
            (ARR2DIM)[idx] = (ARR2DIM)[idx-1] + (COLS);\
          }\
        }

#define array2dim_delete(ARR2DIM)\
        if ((ARR2DIM) != NULL)\
        {\
          free((ARR2DIM)[0]);\
          free(ARR2DIM);\
        }

unsigned long unitedist(const unsigned char *useq,
                        unsigned long ulen,
                        const unsigned char *vseq,
                        unsigned long vlen)
{
  unsigned long i, j, dist, **edtab;

  array2dim_malloc(edtab, ulen+1, vlen+1);
  for (i=0; i <= ulen; i++)
  {
    edtab[i][0] = i;
  }
  for (j=1UL; j <= vlen; j++)
  {
    edtab[0][j] = j;
    for (i=1UL; i <= ulen; i++)
    {
      edtab[i][j] = edtab[i-1][j-1] + ((useq[i-1] == vseq[j-1]) ? 0 : 1);
      if (edtab[i][j] > edtab[i-1][j] + 1)
      {
        edtab[i][j] = edtab[i-1][j] + 1;
      }
      if (edtab[i][j] > edtab[i][j-1] + 1)
      {
        edtab[i][j] = edtab[i][j-1] + 1;
      }
    }
  }
  dist = edtab[ulen][vlen];
  array2dim_delete(edtab);
  return dist;
}
