#ifndef STREE_MINUNIQUEINFO_H
#define STREE_MINUNIQUEINFO_H
#include "core/types_api.h"
#include "core/error_api.h"

int gt_stree_minunique(const char *indexname,
                       GtUword minlength,
                       bool withsequence,
                       GtError *err);

#endif
