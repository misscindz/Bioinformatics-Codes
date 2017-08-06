#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/unused_api.h"
#include "core/init_api.h"
#include "core/ma_api.h"
#include "core/error_api.h"
#include "gt_suffixtree.h"
#include "stree-approx.h"

static void usage(const char *progname)
{
  fprintf(stderr,"Usage: %s <indexname> <patternfile> <differences>\n",
          progname);
}

int main(int argc, char *argv[])
{
  GtError *err;
  GtStree *stree;
  bool haserr = false;
  char *indexname, *patternfile;
  GtUword differences = 0;
  GtWord readlong;

  if (argc != 4 || sscanf(argv[3],GT_WD,&readlong) != 1 || readlong < 0)
  {
    usage(argv[0]);
    return EXIT_FAILURE;
  }
  indexname = argv[1];
  patternfile = argv[2];
  differences = (GtUword) readlong;
  gt_lib_init();
  err = gt_error_new();
  stree = gt_stree_new(indexname,err);
  if (stree == NULL)
  {
    fprintf(stderr,"%s: failure when mapping stree from index %s: %s\n",
            argv[0],indexname,gt_error_get(err));
    haserr = true;
  }
  if (!haserr)
  {
    FILE *fp = fopen(patternfile,"r");
    const GtUword maxpatternlength = 1024;
    GtUword seqnum = 0;
    char pattern[maxpatternlength+1];

    if (fp == NULL)
    {
      fprintf(stderr,"%s: cannot open %s\n",argv[0],patternfile);
      haserr = true;
    }
    printf("# computing complete matches with up to " GT_WU
            " differences\n",differences);
    printf("# indexname(esa)=%s\n",indexname);
    printf("# queryfile=%s\n",patternfile);
    printf("# for each match show: tagnum tagseq dbstartpos abspos \n");
    while (fgets(pattern,maxpatternlength,fp) != NULL)
    {
      GtUword len;
      char *newlineptr = strchr(pattern,'\n');

      gt_assert(newlineptr != NULL);
      len = (GtUword) (newlineptr - pattern);
      if (len > 0 && pattern[0] != '>')
      {
        GtUchar *pattern_t = gt_stree_encode_seq(stree,pattern,len);

        printf("#\t" GT_WU "\t%.*s\n",seqnum,(int) len,pattern);
        stree_approx_pattern_match(stree,
                                   pattern_t,
                                   len,
                                   differences);
        seqnum++;
        gt_free(pattern_t);
      }
    }
    fclose(fp);
  }
  gt_stree_delete(stree);
  gt_error_delete(err);
  if (gt_lib_clean() != 0)
  {
    return GT_EXIT_PROGRAMMING_ERROR;
  }
  return haserr ? EXIT_FAILURE : EXIT_SUCCESS;
}
