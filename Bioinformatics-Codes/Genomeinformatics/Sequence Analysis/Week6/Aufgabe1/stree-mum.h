//GIK Blatt 6
//Aufgabe 1
//Gruppe: Ching, Achoukhi, Breitschuh

#ifndef STREE_MUM_H
#define STREE_MUM_H
#include "core/types_api.h"
#include "core/error_api.h"
#include "gt_suffixtree.h"

typedef struct ProcessData ProcessData;

bool checkLeftMaximality(const GtStree *,
                         const GtUword,
                         const GtUword);

int findMums(void *,
             const GtStree *,
             const GtStreeLoc *);

int gt_stree_mum(const char *,
                 GtUword,
                 bool withsequence,
                 GtError *);

#endif
