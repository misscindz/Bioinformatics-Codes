#ifndef OUTSENSEEDIST_TRIM_H
#define OUTSENSEEDIST_TRIM_H

/* The opaque type of which we only know the name her */

typedef struct TrimParams TrimParams;

/* the constructor */

TrimParams *trimparams_new(unsigned long historysize,
                           unsigned long minimalmatchpercentage,
                           unsigned long frontheadlag);

/* the destructor */

void trimparams_delete(TrimParams *trimparams);

/* run the iteration with increasing value of d. info is
   the pointer by which the TrimParams pointer is passed. So
   Declare and init a local pointer as follows
   TrimParams *trimparams = (TrimParams *) info;
*/

unsigned long outsense_edist_trim(void *info,
                                  const unsigned char *useq,
                                  unsigned long ulen,
                                  const unsigned char *vseq,
                                  unsigned long vlen);

#endif
