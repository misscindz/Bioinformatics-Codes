#ifndef TESTBED_H
#define TESTBED_H
#include <stdbool.h>

typedef unsigned long (*Evaledistfunc)(void *info,
                                       const unsigned char *,
                                       unsigned long,
                                       const unsigned char *,
                                       unsigned long);

void complete_test(Evaledistfunc evaledist,void *info,unsigned long maxlength);

void filesequences_test(Evaledistfunc evaledist,void *info,
                        unsigned long trials,const char *csvfile);

void filepositions_test(Evaledistfunc evaledist,void *info,
                        const char *csvfile,
                        const char *referencefile);

unsigned long compare_distances(Evaledistfunc evaledist,
                                void *info,
                                const unsigned char *useq,
                                unsigned long ulen,
                                const unsigned char *vseq,
                                unsigned long vlen,
                                unsigned long realedist);

#endif
