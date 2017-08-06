#ifndef GT_SUFFIXTREE_H
#define GT_SUFFIXTREE_H
#include "core/types_api.h"
#include "core/error_api.h"

/* The following type implements a suffix tree based on data structures
   from the GenomeTools http://genometools.org.
   We use some of the types implemented in the GenomeTools such as
   GtError for error handling and GtUword for representing unsigned
   integers. In fact, on most machines, GtUword is a synonym for
   unsigned long. To output a value of type GtUword use the format string
   GT_WU instead of %lu. */

typedef struct GtStree GtStree;

/* This is the constructor which takes the name <indexname> of the
   index creates by calling
   ${GTDIR}/bin/gt suffixerator -indexname <indexname> -dna
                                -sds no -des no -md5 no -suf -lcp -tis
                                -db inputfile1 [inputfile2]
   (see index.sh).
   If the constructor fails, it return a NULL-pointer and <err>
   stores an error message, which can be shown by the appropriate
   function of class <GtError>. */

GtStree *gt_stree_new(const char *indexname,GtError *err);

/* This is the destructor for a suffixtree referred to by <stree>. */

void gt_stree_delete(GtStree *stree);

/* The suffix tree encodes the characters of a sequence by integers
   in the range from 0 to <numofchars> - 1 where <numofchars> is returned
   by the following function, given a suffix tree referred to by <stree>. */

unsigned int gt_stree_numofchars(const GtStree *stree);

/* For a suffix tree referred to by <stree>, the following function
   creates a new string storing the
   encoding of the original string <origstring> of length <len>.
   <origstring> must contain characters from the DNA-alphabet in upper or
   lower case. Note that the space allocated for the returned string
   must be freed with <gt_free>. */

GtUchar *gt_stree_encode_seq(const GtStree *stree,
                             const char *origstring,
                             GtUword len);

/* The following function returns
   the encoded character at the given
   <position> in the concatenated string for which the suffixtree <stree>
   was created. */

GtUchar gt_stree_encoded_char(const GtStree *stree,GtUword position);

/* The following function returns the number of sequences in the concatenated
   string for which the suffix tree <stree> was created. */

GtUword gt_stree_num_of_sequences(const GtStree *stree);

/* The following function returns the position of the first separator in the
   concatenation of at least two strings for which the suffix tree <stree>
   was created. */

GtUword gt_stree_separator_first(const GtStree *stree);

/* The following type implements locations in a suffix tree. */

typedef struct GtStreeLoc GtStreeLoc;

/* The following function delivers a new location object. */

GtStreeLoc *gt_stree_loc_new(void);

/* The following function delivers a new location object representing the root
   of the suffix tree referred to by <stree>. */

GtStreeLoc *gt_stree_root_loc_new(const GtStree *stree);

/* This is the destructor for a location <loc>. */

void gt_stree_loc_delete(GtStreeLoc *loc);

/* The following function delivers the depth of the location <loc>, i.e. the
   length of the string it represents. */

GtUword gt_stree_loc_depth(const GtStreeLoc *loc);

/* The following function returns true iff the location <loc> is a
   leaf-location. */

bool gt_stree_loc_is_leaf(const GtStreeLoc *loc);

/* The following function outputs the location <loc> to stdout. */

void gt_stree_loc_show(const GtStreeLoc *loc);

/* The following function returns a pointer to an array storing the
   the leaf-annotations for all leaves in the subtree of the suffix tree
   <stree> below location <loc>. The annotation of a leaf is the
   position where the suffix represented by the leaf starts.
   The returned array must not be freed. */

const GtUword *gt_stree_loc_positions(const GtStree *stree,
                                      const GtStreeLoc *loc);

/* The following function returns the number of leaves
   in the subtree below location <loc>. */

GtUword gt_stree_loc_leaves_count(const GtStreeLoc *loc);

/* The following function requires that the subtree below location <loc>
   in the suffix tree <stree> contains exactly two leaves whose annotations
   (i.e suffix start positions represented by these leaves)
   are stored in the memory area referred to by <startpos_tab>. The
   user of the function is responsible for allocating this memory area. */

void gt_stree_loc_two_leaves(GtUword *startpos_tab,
                             const GtStree *stree,
                             const GtStreeLoc *loc);

/* The following function implements the follow-function for a given
   suffix tree <stree>, an input location <loc_in>, an output location
   <loc_out> and the given character <cc>, which is in the range
   from 0 to <numofchars>-1. Whenever the function returns true,
   <loc_out> stores the location resulting from the follow-function.
   <loc_out> and <loc_in> may refer to the same memory location which
   is useful in cases where the input location is not needed after the
   function call. */

bool gt_stree_loc_follow(const GtStree *stree,
                         GtStreeLoc *loc_out,
                         const GtStreeLoc *loc_in,
                         GtUchar cc);

/* The following function returns an annotation of a leaf in the
   subtree of the suffix tree <stree> below location <loc>. The
   name is justified by the fact that the annotation is a start position
   of a suffix represented by a leaf. */

GtUword gt_stree_loc_suffix_start(const GtStree *stree,
                                  const GtStreeLoc *loc);

/* The following function returns the decoded string represented
   by the location <loc> of the suffix tree <stree>. The pointer
   to the string is dynamically allocated and the user of this function
   needs to free the space using the function <gt_free>. */

char *gt_stree_loc_string(const GtStree *stree,
                          const GtStreeLoc *loc);

/* The following type is used for enumerating locations in the
   suffix tree */

typedef struct GtStreeLocIter GtStreeLocIter;

/* This is the constructor for locations of the suffix tree stored in an index
   named <indexname>. If an error occurred when reading the index,
   the function returns NULL and the <err>-object stores an error message. */

GtStreeLocIter *gt_stree_loc_branch_iter_new(const char *indexname,
                                             GtError *err);

/* This is the destructor for the object of type <GtStreeLocIter>. */

void gt_stree_loc_branch_iter_delete(GtStreeLocIter *gtli);

/* This function extracts the suffix tree from the given suffix tree location
   iterator. */

GtStree *gt_stree_new_from_iter(const GtStreeLocIter *gtli);

/* The following is the type of a function which can be applied
   to any location enumerated by the location iterator */

typedef int (*GtProcessLoc)(void *,const GtStree *,const GtStreeLoc *);

/* The following function applies the function given as second argument
   to all branching location of the suffix tree stored as part of the
   location iterator <gtli>. The third argument is used as the first argument
   of <process_loc>. If an error occurs, the function returns
   a value different from 0 and an error message is stored in
   the given error-object. In case of success, the function returns 0. */

int gt_stree_loc_branch_iter_apply(GtStreeLocIter *gtli,
                                   GtProcessLoc process_loc,
                                   void *process_data,
                                   GtError *err);

#endif
