#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/ma_api.h"
#include "core/init_api.h"
#include "core/error_api.h"
#include "core/chardef.h"
#include "stree-mum.h"

static void usage(const char *progname)
{
  fprintf(stderr,"Usage: %s [-s] <indexname> <minlength>\n",progname);
}

int main(int argc, char *argv[])
{
  GtError *err;
  GtWord readlong;
  bool haserr = false, withsequence = false;
  char *indexname = NULL, *mumlength_string = NULL;

  if (argc == 3)
  {
    withsequence = false;
    indexname = argv[1];
    mumlength_string = argv[2];
  } else
  {
    if (argc == 4)
    {
      if (strcmp(argv[1],"-s") != 0)
      {
        usage(argv[0]);
        return EXIT_FAILURE;
      }
      withsequence = true;
      indexname = argv[2];
      mumlength_string = argv[3];
    } else
    {
      usage(argv[0]);
      return EXIT_FAILURE;
    }
  }
  if (sscanf(mumlength_string,GT_WD,&readlong) != 1 || readlong < 0)
  {
    usage(argv[0]);
    return EXIT_FAILURE;
  }
  gt_lib_init();
  err = gt_error_new();
  if (gt_stree_mum(indexname, (GtUword) readlong, withsequence, err) != 0)
  {
    fprintf(stderr,"%s: failure when enumerating branch locs for "
                   " index %s: %s\n",
                   argv[0],indexname,gt_error_get(err));
    haserr = true;
  }
  gt_error_delete(err);
  if (gt_lib_clean() != 0)
  {
    return GT_EXIT_PROGRAMMING_ERROR;
  }
  return haserr ? EXIT_FAILURE : EXIT_SUCCESS;
}
