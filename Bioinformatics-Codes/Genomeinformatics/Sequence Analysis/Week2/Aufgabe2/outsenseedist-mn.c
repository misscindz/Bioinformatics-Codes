#include <limits.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include "outsenseedist.h"
#include "outsenseedist-trim.h"
#include "testbed.h"
#ifdef WITHTRACE
#include "front-with-trace.h"
#endif

#define MAXHISTORY (sizeof (uint64_t) * CHAR_BIT)

typedef struct
{
  unsigned long wordsize,
                minimalmatchpercentage,
                frontheadlag,
                historysize;
  char *samplepositionsfile, *referencefile, *sequence1, *sequence2;
  bool trimming, verify; 
#ifdef WITHTRACE
  bool witheoplist, withtracepoints;
#endif
} Outsenseoptions;

static void usage(const char *prog,
                  unsigned long default_fhl,
                  unsigned long default_his,
                  unsigned long default_mmp)
{
  fprintf(stderr,"Usage: %s options\n"
          "    -t use trimming method\n"
#ifdef WITHTRACE
          "    -e compute edit operation list (does not work with trimming)\n"
          "    -p compute and verify tracepoints after generating alignment without trimming\n"
#endif
          "    -w <length of word to generate>\n"
          "    -r <referencefile>\n"
          "    -s <file with pair of sample positions>\n"
          "    -g <sequence1> <sequence2>\n"
          "    -d <maximal lag to front head>, default %lu\n"
          "    -h <length of match history <= %lu>, default %lu\n"
          "    -m <minimal percentage of matches in history>, default %lu%%\n"
          "    -v verify intermediate results after generating alignment without trimming\n",
          prog,
          default_fhl,
          MAXHISTORY,
          default_his,
          default_mmp);
}

#define USAGE usage(argv[0],default_fhl,default_his,default_mmp);\
              exit(EXIT_FAILURE);

static void parse_options(Outsenseoptions *options,
                          int argc, char * const argv[])
{
  int opt;
  long readlong;
  const unsigned long default_his = 60UL,
                      default_mmp = 55UL, default_fhl = 30UL;
  const char *optioncharacters = "tewrsgdhmpv";

  options->wordsize = 0;
  options->referencefile = NULL;
  options->samplepositionsfile = NULL;
  options->referencefile = NULL;
  options->sequence1 = NULL;
  options->sequence2 = NULL;
  options->frontheadlag = default_fhl;
  options->historysize = default_his;
  options->minimalmatchpercentage = default_mmp;
  options->verify = false;
  options->trimming = false;
#ifdef WITHTRACE
  options->witheoplist = false;
  options->withtracepoints = false;
#endif
  if (argc == 1)
  {
    USAGE;
  }
  while (true)
  {
    opt = getopt(argc, argv, optioncharacters);
    if (opt == -1)
    {
      break;
    }
    switch ((char) opt)
    {
      case 's':
      case 'r':
        if (optind >= argc)
        {
          fprintf(stderr,"%s: missing argument to option -%c\n",argv[0],
                    (char) opt);
          USAGE;
        }
        if ((char) opt == 'r')
        {
          options->referencefile = argv[optind];
        } else
        {
          assert((char) opt == 's');
          options->samplepositionsfile = argv[optind];
        }
        optind++;
        break;
      case 'g':
        if (optind + 1 >= argc)
        {
          fprintf(stderr,"%s: missing argument to option -%c\n",argv[0],
                    (char) opt);
          USAGE;
        }
        options->sequence1 = strdup(argv[optind]);
        options->sequence2 = strdup(argv[optind+1]);
        optind+=2;
        break;
      case 'v':
        options->verify = true;
#ifdef WITHTRACE
        options->witheoplist = true;
#endif
        break;
#ifdef WITHTRACE
      case 'p':
        options->withtracepoints = true;
        options->witheoplist = true;
        break;
#endif
      case 't':
        options->trimming = true;
        break;
#ifdef WITHTRACE
      case 'e':
        options->witheoplist = true;
        break;
#endif
      default:
        if (strchr(optioncharacters,opt) == NULL)
        {
          fprintf(stderr,"%s: illegal argument to option -%c\n",argv[0],
                  (char) opt);
          USAGE;
        } else
        {
          if (optind >= argc || sscanf(argv[optind],"%ld",&readlong) != 1 ||
              readlong < 1)
          {
            fprintf(stderr,"%s: option -%c needs positive integer argument\n",
                    argv[0],(char) opt);
            USAGE;
          }
          switch((char) opt)
          {
            case 'm':
              if (readlong > 100L)
              {
                fprintf(stderr,"%s: match percentage must not exceed 100\n",
                        argv[0]);
                USAGE;
              }
              options->minimalmatchpercentage = (unsigned long) readlong;
              break;
            case 'h':
              if (readlong > (long) MAXHISTORY)
              {
                fprintf(stderr,"%s: history must not exceed %lu\n",argv[0],
                        MAXHISTORY);
                USAGE;
              }
              options->historysize = (unsigned long) readlong;
              break;
            case 'd':
              options->frontheadlag = (unsigned long) readlong;
              break;
            case 'w':
              options->wordsize = (unsigned long) readlong;
              if (options->wordsize == 0)
              {
                fprintf(stderr,"%s: option -w needs positive integer "
                               "rgument\n",argv[0]);
                USAGE;
              }
              break;
            default:
              fprintf(stderr,"%s: illegal option -%c\n",argv[0],(char) opt);
              USAGE;
          }
          optind++;
        }
    }
  }
  if (optind != argc)
  {
    fprintf(stderr,"%s: superflous options\n",argv[0]);
    USAGE;
  }
  if (options->sequence1 != NULL)
  {
    assert(options->sequence2 != NULL);
  } else
  {
    if (options->samplepositionsfile != NULL &&
        options->referencefile == NULL)
    {
      fprintf(stderr,"%s: if option -s is used then option -r is mandatory\n",
              argv[0]);
      USAGE;
    }
    if (options->wordsize > 0 &&
        options->referencefile != NULL)
    {
      fprintf(stderr,"%s: option -w and -r are incompatible\n",argv[0]);
      USAGE;
    }
    if (options->wordsize > 0 &&
        options->sequence1 != NULL)
    {
      fprintf(stderr,"%s: option -w and -g are incompatible\n",argv[0]);
      USAGE;
    }
    if (options->referencefile > 0 &&
        options->sequence1 != NULL)
    {
      fprintf(stderr,"%s: option -r and -g are incompatible\n",argv[0]);
      USAGE;
    }
#ifdef WITHTRACE
    if (options->withtracepoints && options->verify)
    {
      fprintf(stderr,"%s: option -v and -p are incompatible\n",argv[0]);
      USAGE;
    }
#endif
    if (options->wordsize == 0 &&
        options->referencefile == NULL &&
        options->sequence1 == NULL)
    {
      fprintf(stderr,"%s: use either option -w or -r\n",argv[0]);
      USAGE;
    }
#ifdef WITHTRACE
    if (options->trimming && options->witheoplist)
    {
      fprintf(stderr,"%s: option -t is not compatible with option -e and -c\n",
              argv[0]);
      USAGE;
    }
#endif
  }
}

int main(int argc,char *argv[])
{
  Outsenseoptions options;
  void *info;
  Evaledistfunc callback;
#ifdef WITHTRACE
  FrontEdistTracewitheoplist fet_eoplist = {NULL,NULL};
#endif

  parse_options(&options,argc,argv);
  if (options.trimming)
  {
#ifdef WITHTRACE
    assert(!options.witheoplist);
#endif
    info = (void *) trimparams_new(options.historysize,
                                   options.minimalmatchpercentage,
                                   options.frontheadlag);
    callback = outsense_edist_trim;
  } else
  {
#ifdef WITHTRACE
    if (options.witheoplist)
    {
      if (options.verify || options.withtracepoints)
      {
        fet_eoplist.fet = front_edist_trace_new();
        fet_eoplist.eoplist = gt_eoplist_new();
        info = &fet_eoplist;
        callback = options.verify ? front_edist_trace_eoplist_void
                                  : front_edist_segments_void;
      } else
      {
        info = front_edist_trace_new();
        callback = front_edist_trace_distance_void;
      }
    } else
    {
      info = NULL;
      callback = outsense_edist;
    }
#else
    info = NULL;
    callback = outsense_edist;
#endif
  }
  if (options.wordsize > 0)
  {
    printf("# compare all pairs of binary sequences of length up to %ld%s\n",
           options.wordsize,
           options.trimming ? " with trimming" : "");
    complete_test(callback,info,options.wordsize);
  } else
  {
    if (options.referencefile != NULL)
    {
      if (options.samplepositionsfile == NULL)
      {
        filesequences_test(callback,info,1000,options.referencefile);
      } else
      {
        filepositions_test(callback,info,options.samplepositionsfile,
                           options.referencefile);
      }
    } else
    {
      assert(options.sequence1 != NULL && options.sequence2 != NULL);
      (void) compare_distances(callback,
                               info,
                               (const unsigned char *) options.sequence1,
                               (unsigned long) strlen(options.sequence1),
                               (const unsigned char *) options.sequence2,
                               (unsigned long) strlen(options.sequence2),
                               ULONG_MAX);
      free(options.sequence1);
      free(options.sequence2);
    }
  }
  if (options.trimming)
  {
    assert(info != NULL);
    trimparams_delete((TrimParams *) info);
  } else
  {
#ifdef WITHTRACE
    if (options.witheoplist)
    {
      if (options.verify || options.withtracepoints)
      {
        front_edist_trace_delete(fet_eoplist.fet);
        gt_eoplist_delete(fet_eoplist.eoplist);
      } else
      {
        front_edist_trace_delete((FrontEdistTrace *) info);
      }
    }
#endif
  }
  return EXIT_SUCCESS;
}
