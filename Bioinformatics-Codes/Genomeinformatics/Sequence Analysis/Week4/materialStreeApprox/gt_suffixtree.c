#include <stdbool.h>
#include "core/unused_api.h"
#include "core/encseq_api.h"
#include "match/sarr-def.h"
#include "match/esa-map.h"
#include "match/esa-splititv.h"
#include "match/lcpinterval.h"
#include "match/esa-seqread.h"
#include "match/esa-dfs.h"
#include "gt_suffixtree.h"

struct GtStree
{
  GtUword totallength;
  Suffixarray *suffixarray;
};

GtStree *gt_stree_new(const char *indexname,GtError *err)
{
  GtStree *stree = gt_malloc(sizeof *stree);

  stree->suffixarray = gt_malloc(sizeof *stree->suffixarray);
  if (gt_mapsuffixarray(stree->suffixarray,
                        SARR_ESQTAB | SARR_SUFTAB,
                        indexname,
                        NULL,
                        err) != 0)
  {
    gt_freesuffixarray(stree->suffixarray);
    gt_free(stree);
    return NULL;
  }
  stree->totallength
    = gt_encseq_total_length(stree->suffixarray->encseq);
  return stree;
}

void gt_stree_delete(GtStree *stree)
{
  if (stree != NULL)
  {
    if (stree->suffixarray != NULL)
    {
      gt_freesuffixarray(stree->suffixarray);
      gt_free(stree->suffixarray);
    }
    gt_free(stree);
  }
}

GtUchar *gt_stree_encode_seq(const GtStree *stree,
                             const char *origstring,
                             GtUword len)
{
  GtUchar *transformed_string = gt_malloc(len * sizeof *transformed_string);
  GtAlphabet *alphabet = gt_encseq_alphabet(stree->suffixarray->encseq);

  gt_alphabet_encode_seq(alphabet, transformed_string,origstring,len);
  return transformed_string;
}

unsigned int gt_stree_numofchars(const GtStree *stree)
{
  GtAlphabet *alphabet = gt_encseq_alphabet(stree->suffixarray->encseq);
  return gt_alphabet_num_of_chars(alphabet);
}

GtUword gt_stree_num_of_sequences(const GtStree *stree)
{
  return gt_encseq_num_of_sequences(stree->suffixarray->encseq);
}

GtUword gt_stree_separator_first(const GtStree *stree)
{
  gt_assert(stree != NULL && stree->suffixarray != NULL &&
            stree->suffixarray->encseq != NULL &&
            gt_stree_num_of_sequences(stree) > 1UL);
  return gt_encseq_seqlength(stree->suffixarray->encseq,0);
}

GtUchar gt_stree_encoded_char(const GtStree *stree,GtUword position)
{
  gt_assert(stree != NULL && stree->suffixarray != NULL &&
            stree->suffixarray->encseq != NULL);
  return gt_encseq_get_encoded_char(stree->suffixarray->encseq,
                                    position,
                                    GT_READMODE_FORWARD);
}

struct GtStreeLoc
{
  Lcpinterval itv;
};

static GtStreeLoc *gt_stree_loc_new_gen(bool for_root,const GtStree *stree)
{
  GtStreeLoc *loc = gt_malloc(sizeof *loc);

  loc->itv.offset = 0;
  loc->itv.left = 0;
  if (for_root)
  {
    gt_assert(stree != NULL);
    loc->itv.right = stree->totallength;
  } else
  {
    gt_assert(stree == NULL);
    loc->itv.right = 0;
  }
  return loc;
}

GtStreeLoc *gt_stree_root_loc_new(const GtStree *stree)
{
  return gt_stree_loc_new_gen(true,stree);
}

GtStreeLoc *gt_stree_loc_new(void)
{
  return gt_stree_loc_new_gen(false,NULL);
}

void gt_stree_loc_show(const GtStreeLoc *loc)
{
  printf(GT_WU "-[" GT_WU "," GT_WU "]\n",
         loc->itv.offset,loc->itv.left,loc->itv.right);
}

void gt_stree_loc_delete(GtStreeLoc *loc)
{
  if (loc != NULL)
  {
    gt_free(loc);
  }
}

const GtUword *gt_stree_loc_positions(const GtStree *stree,
                                      const GtStreeLoc *loc)
{
  gt_assert(stree != NULL && loc != NULL);
  return stree->suffixarray->suftab + loc->itv.left;
}

GtUword gt_stree_loc_leaves_count(const GtStreeLoc *loc)
{
  gt_assert(loc != NULL && loc->itv.left <= loc->itv.right);
  return loc->itv.right - loc->itv.left + 1;
}

GtUword gt_stree_loc_depth(const GtStreeLoc *loc)
{
  return loc->itv.offset;
}

bool gt_stree_loc_is_leaf(const GtStreeLoc *loc)
{
  gt_assert(loc->itv.left <= loc->itv.right);
  return loc->itv.left == loc->itv.right ? true : false;
}

bool gt_stree_loc_follow(const GtStree *stree,
                         GtStreeLoc *loc_out,
                         const GtStreeLoc *loc_in,
                         GtUchar cc)
{
  Simplelcpinterval simple_loc_out;
  bool occurs = gt_lcpintervalfindcharchildintv(stree->suffixarray->encseq,
                                         stree->suffixarray->readmode,
                                         stree->totallength,
                                         stree->suffixarray->suftab,
                                         &simple_loc_out,
                                         cc,
                                         loc_in->itv.offset,
                                         loc_in->itv.left,
                                         loc_in->itv.right);
  loc_out->itv.left = simple_loc_out.left;
  loc_out->itv.right = simple_loc_out.right;
  loc_out->itv.offset = loc_in->itv.offset + 1;
  return occurs;
}

GtUword gt_stree_loc_suffix_start(const GtStree *stree,
                                  const GtStreeLoc *loc)
{
  gt_assert(stree != NULL && stree->suffixarray != NULL &&
            loc != NULL && loc->itv.left <= loc->itv.right &&
            loc->itv.offset > 0);
  return stree->suffixarray->suftab[loc->itv.left];
}

char *gt_stree_loc_string(const GtStree *stree,
                          const GtStreeLoc *loc)
{
  char *buffer;
  GtUword startpos, depth;

  gt_assert(stree != NULL &&
            stree->suffixarray != NULL &&
            stree->suffixarray->encseq != NULL &&
            loc != NULL && loc->itv.left <= loc->itv.right &&
            loc->itv.offset > 0);
  startpos = gt_stree_loc_suffix_start(stree,loc);
  depth = loc->itv.offset;

  buffer = gt_malloc((depth + 1) * sizeof *buffer);
  gt_encseq_extract_decoded(stree->suffixarray->encseq,
                            buffer,
                            startpos,
                            startpos + loc->itv.offset - 1);
  buffer[depth] = '\0';
  return buffer;
}

GtStreeLoc **gt_stree_loc_leaves(const GtStree *stree,const GtStreeLoc *loc)
{
  GtStreeLoc **loc_tab;
  GtArrayBoundswithchar bwci;
  GtUword idx, numleaves, leafcount = 0;

  gt_assert(stree != NULL &&
            stree->suffixarray != NULL &&
            stree->suffixarray->encseq != NULL && loc != NULL);
  numleaves = gt_stree_loc_leaves_count(loc);
  loc_tab = gt_malloc(numleaves * sizeof *loc_tab);
  bwci.allocatedBoundswithchar = numleaves + 1;
  bwci.nextfreeBoundswithchar = 0;
  bwci.spaceBoundswithchar
     = gt_malloc(bwci.allocatedBoundswithchar *
                 sizeof * bwci.spaceBoundswithchar);
  gt_lcpintervalsplitwithoutspecial(&bwci,
                                    stree->suffixarray->encseq,
                                    stree->suffixarray->readmode,
                                    stree->totallength,
                                    stree->suffixarray->suftab,
                                    loc->itv.offset,
                                    loc->itv.left,
                                    loc->itv.right);
  for (idx = 0; idx < bwci.nextfreeBoundswithchar; idx++)
  {
    if (bwci.spaceBoundswithchar[idx].lbound ==
        bwci.spaceBoundswithchar[idx].rbound)
    {
      GtStreeLoc *loc_b = gt_stree_loc_new();

      loc_b->itv.offset = loc->itv.offset + 1;
      loc_b->itv.left = loc_b->itv.right = bwci.spaceBoundswithchar[idx].lbound;
      gt_assert(leafcount < numleaves);
      loc_tab[leafcount++] = loc_b;
    }
  }
  if (leafcount < numleaves)
  {
    GtUword b;
    gt_assert(bwci.nextfreeBoundswithchar > 0 &&
              bwci.spaceBoundswithchar[bwci.nextfreeBoundswithchar-1].rbound
              < loc->itv.right);
    for (b = bwci.spaceBoundswithchar[bwci.nextfreeBoundswithchar-1].rbound+1;
         b <= loc->itv.right; b++)
    {
      GtStreeLoc *loc_b = gt_stree_loc_new();

      loc_b->itv.offset = loc->itv.offset + 1;
      loc_b->itv.left = loc_b->itv.right = b;
      gt_assert(leafcount < numleaves);
      loc_tab[leafcount++] = loc_b;
    }
    gt_assert(leafcount == numleaves);
  }
  GT_FREEARRAY(&bwci,Boundswithchar);
  return loc_tab;
}

void gt_stree_loc_two_leaves(GtUword *startpos_tab,
                             const GtStree *stree,
                             const GtStreeLoc *loc)
{
  GtStreeLoc leafloc;

  gt_assert(stree != NULL &&
            stree->suffixarray != NULL &&
            stree->suffixarray->encseq != NULL && loc != NULL);
  gt_assert(gt_stree_loc_leaves_count(loc) == 2UL);
  leafloc.itv.offset = loc->itv.offset;
  leafloc.itv.left = leafloc.itv.right = loc->itv.left;
  startpos_tab[0] = gt_stree_loc_suffix_start(stree,&leafloc);
  leafloc.itv.left = leafloc.itv.right = loc->itv.right;
  startpos_tab[1] = gt_stree_loc_suffix_start(stree,&leafloc);
}

typedef struct
{
  Lcpinterval lastcompletenode;
  GtStree stree;
  GtProcessLoc processlcpinterval;
  void *processinfo;
} Elcpstate;

static Dfsinfo *allocateDfsinfo_elcp(GT_UNUSED Dfsstate *astate)
{
  return (Dfsinfo *) gt_malloc(sizeof (Lcpinterval));
}

static void freeDfsinfo_elcp(Dfsinfo *adfsinfo,GT_UNUSED Dfsstate *state)
{
  gt_free((Lcpinterval *) adfsinfo);
}

static int processcompletenode_elcp(GtUword nodeptrdepth,
                                    Dfsinfo *anodeptr,
                                    GT_UNUSED GtUword nodeptrminusonedepth,
                                    Dfsstate *astate,
                                    GT_UNUSED GtError *err)
{
  Lcpinterval *nodeptr = (Lcpinterval *) anodeptr;
  Elcpstate *state = (Elcpstate *) astate;

  gt_assert(state != NULL);
  gt_assert(nodeptr != NULL);
  nodeptr->offset = state->lastcompletenode.offset = nodeptrdepth;
  state->lastcompletenode.left = nodeptr->left;
  state->lastcompletenode.right = nodeptr->right;
  if (state->processlcpinterval != NULL)
  {
    GtStreeLoc loc;

    loc.itv = state->lastcompletenode;
    if (state->processlcpinterval(state->processinfo,
                                  &state->stree,
                                  &loc) != 0)
    {
      return -1;
    }
  }
  return 0;
}

static void assignleftmostleaf_elcp(Dfsinfo *adfsinfo,
                                    GtUword leftmostleaf,
                                    GT_UNUSED Dfsstate *dfsstate)
{
  ((Lcpinterval *) adfsinfo)->left = leftmostleaf;
}

static void assignrightmostleaf_elcp(Dfsinfo *adfsinfo,
                                     GtUword currentindex,
                                     GT_UNUSED GtUword previoussuffix,
                                     GT_UNUSED GtUword currentlcp,
                                     GT_UNUSED Dfsstate *dfsstate)
{
  ((Lcpinterval *) adfsinfo)->right = currentindex;
}

static int gt_processlcpintervals(Sequentialsuffixarrayreader *ssar,
                                  GtProcessLoc processfunction,
                                  void *processinfo,
                                  GtError *err)
{
  Elcpstate *state;
  bool haserr = false;

  state = gt_malloc(sizeof (*state));
  state->processlcpinterval = processfunction;
  state->processinfo = processinfo;
  state->stree.suffixarray = ssar->suffixarray;
  state->stree.totallength
    = gt_encseq_total_length(ssar->suffixarray->encseq);
  if (gt_depthfirstesa(ssar,
                       allocateDfsinfo_elcp,
                       freeDfsinfo_elcp,
                       NULL,
                       NULL,
                       processcompletenode_elcp,
                       assignleftmostleaf_elcp,
                       assignrightmostleaf_elcp,
                       (Dfsstate *) state,
                       NULL,
                       err) != 0)
  {
    haserr = true;
  }
  if (!haserr && state->processlcpinterval != NULL)
  {
    GtStreeLoc loc;

    state->lastcompletenode.offset = 0;
    state->lastcompletenode.left = 0;
    state->lastcompletenode.right
      = gt_Sequentialsuffixarrayreader_totallength(ssar);
    loc.itv = state->lastcompletenode;
    if (state->processlcpinterval(state->processinfo,
                                  &state->stree,
                                  &loc) != 0)
    {
      haserr = true;
    }
  }
  gt_free(state);
  return haserr ? -1 : 0;
}

struct GtStreeLocIter
{
  Sequentialsuffixarrayreader *ssar;
};

GtStreeLocIter *gt_stree_loc_branch_iter_new(const char *indexname,
                                             GtError *err)
{
  GtStreeLocIter  *gtli;

  gt_error_check(err);
  gtli = gt_malloc(sizeof *gtli);
  gtli->ssar = gt_newSequentialsuffixarrayreaderfromfile(indexname,
                                                         SARR_LCPTAB |
                                                         SARR_SUFTAB |
                                                         SARR_ESQTAB,
                                                         false,
                                                         NULL,
                                                         err);
  if (gtli->ssar == NULL)
  {
    gt_free(gtli);
    return NULL;
  }
  return gtli;
}

GtStree *gt_stree_new_from_iter(const GtStreeLocIter *gtli)
{
  GtStree *stree;

  gt_assert(gtli != NULL && gtli->ssar != NULL && gtli->ssar->suffixarray);
  stree = gt_malloc(sizeof *stree);
  stree->suffixarray = gtli->ssar->suffixarray;
  stree->totallength
    = gt_encseq_total_length(stree->suffixarray->encseq);
  return stree;
}

int gt_stree_loc_branch_iter_apply(GtStreeLocIter *gtli,
                                   GtProcessLoc process_loc,
                                   void *process_data,
                                   GtError *err)
{
  gt_assert(gtli != NULL && gtli->ssar != NULL);
  gt_error_check(err);
  if (gt_processlcpintervals(gtli->ssar, process_loc, process_data,
                             err) != 0)
  {
    gt_freeSequentialsuffixarrayreader(&gtli->ssar);
    return -1;
  }
  return 0;
}

void gt_stree_loc_branch_iter_delete(GtStreeLocIter *gtli)
{
  if (gtli != NULL && gtli->ssar != NULL)
  {
    gt_freeSequentialsuffixarrayreader(&gtli->ssar);
    gt_free(gtli);
  }
}
