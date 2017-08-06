#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "readinput.h"

unsigned char *readinputfile(unsigned long *filesize,const char *filename)
{
  long filesize_long;
  unsigned char *filecontent;
  FILE *fp = fopen(filename,"r");

  if (fp == NULL)
  {
    return NULL;
  }
  fseek (fp, 0, SEEK_END); /* jump to end of file */
  filesize_long = ftell(fp); /* read current position */
  rewind(fp); /* continue at the start of the file */
  assert(filesize_long >= 0);
  filecontent = malloc(sizeof *filecontent * filesize_long);
  assert(filecontent != NULL);
  if (fread(filecontent,sizeof *filecontent,filesize_long,fp) != filesize_long)
  {
    free(filecontent);
    fclose(fp);
    return NULL;
  }
  *filesize = (unsigned long) filesize_long;
  fclose(fp);
  return filecontent;
}
