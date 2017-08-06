#ifndef OUTSENSEEDIST_H
#define OUTSENSEEDIST_H
unsigned long outsense_edist(void *info,
                             const unsigned char *useq,
                             unsigned long ulen,
                             const unsigned char *vseq,
                             unsigned long vlen);

unsigned long outsense_edist_inplace(const unsigned char *useq,
                                     unsigned long ulen,
                                     const unsigned char *vseq,
                                     unsigned long vlen);
#endif
