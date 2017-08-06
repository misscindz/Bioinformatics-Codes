#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include "testbed.h"
#include "unitedist.h"
#include "readinput.h"
#include "word_enum.h"

unsigned long compare_distances(Evaledistfunc evaledist,
                                void *info,
                                const unsigned char *useq,
                                unsigned long ulen,
                                const unsigned char *vseq,
                                unsigned long vlen,
                                unsigned long realedist)
{
  unsigned long myedist = evaledist(info,useq,ulen,vseq,vlen);

#undef CORRECTED_OUTPUT
#ifdef CORRECTED_OUTPUT
  realedist = unitedist(useq,ulen,vseq,vlen);
#else
  if (realedist == ULONG_MAX)
  {
    realedist = unitedist(useq,ulen,vseq,vlen);
  }
#endif
  if (myedist != realedist)
  {
    printf("%lu != %lu (correct), computed sim=%.2f, correct sim=%.2f\n",
            myedist,realedist,
            100.0 * (1.0 - (2.0 * myedist)/(ulen+vlen)),
            100.0 * (1.0 - (2.0 * realedist)/(ulen+vlen)));
    if (myedist != ulen + vlen + 1 && myedist < realedist)
    {
      fprintf(stderr,"%*s %*s: %lu != %lu (correct)\n",
                       (int) ulen,useq,(int) vlen,vseq,
                       myedist,realedist);
      exit(EXIT_FAILURE);
    }
  }
  return myedist;
}

void complete_test(Evaledistfunc evaledist,void *info,
                   unsigned long maxlength)
{
  unsigned long len1, countcomparisons = 0;

  for (len1 = 1UL; len1 <= maxlength; len1++)
  {
    unsigned long len2;
    for (len2 = 1UL; len2 <= maxlength; len2++)
    {
      const unsigned char *word1;
      Wordenumerator *we1 = word_enumerator_new(len1);
      Wordenumerator *we2 = word_enumerator_new(len2);

      while ((word1 = word_enumerator_next(we1)) != NULL)
      {
        const unsigned char *word2;

        while ((word2 = word_enumerator_next(we2)) != NULL)
        {
          (void) compare_distances(evaledist,info,word1,len1,word2,len2,
                                   ULONG_MAX);
          countcomparisons++;
        }
        word_enumerator_reset(we2);
      }
      word_enumerator_delete(we2);
      word_enumerator_delete(we1);
    }
  }
  printf("# %lu correct comparisons\n",countcomparisons);
}

void filesequences_test(Evaledistfunc evaledist,void *info,
                        unsigned long trials,const char *csvfile)
{
  const unsigned long minlength = 10, maxlength = 200;
  unsigned long filesize;
  unsigned char *filecontent = readinputfile(&filesize,csvfile);

  if (filecontent == NULL)
  {
    fprintf(stderr,"cannot read file \"%s\"\n",csvfile);
    exit(EXIT_FAILURE);
  }
  if (filesize >= minlength)
  {
    unsigned long t;

    printf("# compare %lu pairs of substrings from file %s, length range is "
           "[%lu,%lu]\n",trials,csvfile,minlength,maxlength);
    for (t = 0; t < trials; t++)
    {
      unsigned long pos1, pos2, len1, len2;

      pos1 = drand48() * (filesize - minlength);
      len1 = 1 + drand48() * maxlength;
      if (pos1 + len1 >= filesize)
      {
        len1 = filesize - pos1 - 1;
        assert(len1 > 0 && pos1 + len1 < filesize);
      }
      pos2 = drand48() * (filesize - minlength);
      len2 = len1 + drand48() * 10;
      if (pos2 + len2 >= filesize)
      {
        len2 = filesize - pos2 - 1;
        assert(len2 > 0 && pos2 + len2 < filesize);
      }
      (void) compare_distances(evaledist,info,
                               filecontent + pos1,len1,
                               filecontent + pos2,len2,ULONG_MAX);
    }
  }
  free(filecontent);
}

void filepositions_test(Evaledistfunc evaledist,void *info,
                        const char *csvfile,const char *referencefile)
{
  FILE *fp;
  unsigned long filesize, linenum = 1UL;
  unsigned char *filecontent = readinputfile(&filesize,referencefile);
  long readstart1, readstart2, readlength1, readlength2, edist;

  fp = fopen(csvfile,"rb");
  if (fp == NULL)
  {
    fprintf(stderr,"cannot open %s\n",csvfile);
    exit(EXIT_FAILURE);
  }
  for (linenum = 1UL;
       fscanf(fp,"%ld %ld %ld %ld %ld",
                 &readstart1,
                 &readlength1,
                 &readstart2,
                 &readlength2,
                 &edist) == 5; linenum++)
  {
    unsigned long realedist;
    if (readstart1 < 0 || readlength1 < 1L || readstart2 < 0 ||
        readlength2 < 1L || edist < 0)
    {
      fprintf(stderr,
              "file %s, line %lu: expect five non-negative integers of "
              "which the second and fourth must be strictly positive\n",
              csvfile,linenum);
      exit(EXIT_FAILURE);
    }
    if (readstart1 + readlength1 > filesize)
    {
      fprintf(stderr,
              "file %s, line %lu: illegal position range %lu %lu in file "
              "of size %lu\n",
              csvfile,linenum,readstart1,readstart1 + readlength1 - 1,filesize);
      exit(EXIT_FAILURE);
    }
    if (readstart2 + readlength2 > filesize)
    {
      fprintf(stderr,
              "file %s, line %lu: illegal position range %lu %lu in file "
              "of size %lu\n",
              csvfile,linenum,readstart2,readstart2 + readlength2 - 1,filesize);
      exit(EXIT_FAILURE);
    }
    realedist
      = compare_distances(evaledist,info,
                          filecontent + readstart1,(unsigned long) readlength1,
                          filecontent + readstart2,(unsigned long) readlength2,
                          (unsigned long) edist);
    printf("%ld\t%ld\t%ld\t%ld\t%lu\n",
           readstart1,readlength1,
           readstart2,readlength2,realedist);
  }
  fclose(fp);
  free(filecontent);
}
