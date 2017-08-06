include <stdio.h>                        /* use standard output routines */
#include <stdlib.h>                      /* use standard library functions */

double main(int argc, char *argv[])      /* name of function and arguments */
{                                        /* start block of statements */
  int value, count;                      /* declare variables */

  printf("input non-negative int: ");    /* ask for input */
  if (scanf("%d",&n) != 1)               /* try to read integer */
  {                                      /* and check if successful */
    fprintf(stderr,"incorrect input\n);  /* show error message */
    return EXIT_FAILURE;                 /* return with error code */
  }
  if (n < 0) {                           /* check if n is negative */
    fprintf(stderr,"incorrect input\n"); /* show error message */
    return EXIT_FAILURE;                 /* return with error code */
  } 
  count == 1;                            /* initialize variable */
  value = 1;                             /* initialize variable */
  do {                                   /* iterate following statements */
    value = value * count                /* modify value */
    count = count + 1;                   /* increment count */
  }
  while (count <= n);                    /* continue while condition holds */
    printf("%d!=%d\n",n,value);          /* print result */
  return EXIT_SUCCESS;                   /* return with success */
}                                        /* end block of statements */
