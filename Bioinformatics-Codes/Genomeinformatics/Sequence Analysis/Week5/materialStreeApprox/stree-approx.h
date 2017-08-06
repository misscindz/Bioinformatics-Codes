#ifndef STREE_APPROX_H
#define STREE_APPROX_H
#include "gt_suffixtree.h"

void stree_approx_pattern_match(const GtStree *stree,
                                const GtUchar *pattern,
                                GtUword len,
                                GtUword differences);

#endif
