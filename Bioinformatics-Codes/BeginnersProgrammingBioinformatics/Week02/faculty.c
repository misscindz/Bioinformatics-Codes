#include <stdio.h>                        /* "#" fehlt */
#include <stdlib.h>


int main(int argc, char *argv[])         /* "double" in "int" umgeaendert, denn main gibt nur integer Werte aus*/
{
  int value, count, n;

  printf("input non-negative int: ");
  if (scanf("%d",&n) != 1)
  {
    fprintf(stderr,"incorrect input\n"); /* Gaensefuesschen hinzugefuegt */
    return EXIT_FAILURE;
  }
  if (n < 0)
  {
    fprintf(stderr,"incorrect input\n");
    return EXIT_FAILURE;
  }
  count = 1;                             /* ein Gleichzeichen zu viel */
  value = 1;
  do {
    value = value * count;
    count = count + 1;
  }
  while (count <= n);
    printf("%d!=%d\n",n,value);
  return EXIT_SUCCESS;
}
